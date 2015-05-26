/* 
   Copyright (c) 2014-2015 ostrichserver


   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991, or
   (at your option) version 3 dated 29 June, 2007.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
     
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "config.h"
#include "application.h"
#include "signals.h"

int  workerid;
int  workerstatus;
int  masterid;

int show_version;
int show_help;
int quiet;
int reconfig;
int restart;
int cf_daemon = 1;

char  *old_argv_last;
char **old_argv;
char **new_argv;
extern char **environ;

char *master_title = "manager";
char *worker_title = "worker";

int os_parse_options(int argc, char **argv)
{
    int   i;
    char *p;

    for (i = 1; i < argc; i++) {
        p = argv[i];

        if (*p++ != '-') {
            return OS_ERR;
        }

        while (*p) {
            switch (*p++) {
                case '?':
                case 'h':
                    show_help = 1;
                    break;
                case 'v':
                case 'V':
                    show_version = 1;
                    break;
                //case 'q':
                //    quiet = 1;
                //    break;
                //case 'r':
                //    reconfig = 1;
                //    break;
                //case 's':
                //    restart = 1;
                case 't':
                    cf_daemon = 0;
                    break;
            }
        }

        continue;
    }

    return OS_OK;
}

int os_worker_start()
{
    int          result = 0;
    record      *rec;
    int          listenfd = 0;
    int          rc = 0;

    if (cf_daemon) {    
        rc = fork();
        if (rc > 0) {
            return OS_OK;
        }

        _proctitle_setting(worker_title);
    }
    
    result = config_parse_file("record.config");

    eventprocessor_init(DEFAULT_THREADS);

    application_protocol_init("HTTP");
    
    acceptor_init();

    if (cf_daemon) {
        for (;;) {
            sleep(1);
            //event_notify_wait(); // todo process errno
        }
    }
}

int _proctitle_setting(char *title)
{
    char  *p;
    char  *head = "ostrichserver: ";
    
    old_argv[1] = NULL;

    p = old_argv[0];

    p = strncpy(p, "ostrichserver: ", old_argv_last - old_argv[0]);

    p += strlen(head);

    p = strncpy(p, title, old_argv_last - p);

    p += sizeof(title);

    if (old_argv_last - p) {
        memset(p, '\0', old_argv_last - p);
    }

    return OS_OK;
}

int os_init_proctitle()
{
    char    *p;
    size_t   size = 0;
    int      i;

    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    p  = os_calloc(size);

    old_argv_last = old_argv[0];

    for (i = 0; old_argv[i]; i++) {
        if (old_argv_last == old_argv[i]) {
            old_argv_last = old_argv[i] + strlen(old_argv[i]) + 1;
        }
    }

    for (i = 0; environ[i]; i++) {
        if (old_argv_last == environ[i]) {
            size = strlen(environ[i]) + 1;
            old_argv_last = environ[i] + size;

            strncpy(p, environ[i], size);
            environ[i] = p;
            p += size;
        }
    }

    old_argv_last--;

    return OS_OK;
}

int os_save_argv(int argc, char **argv)
{
    int       i;
    size_t    len;
    
    old_argv = argv;

    new_argv = os_calloc((argc + 1) * sizeof(char *));

    for (i = 0; i < argc; i++) {
        
        len = strlen(argv[i]) + 1;

        new_argv[i] = os_calloc(len);
        
        strncpy(new_argv[i], old_argv[i], len);
    }

    new_argv[i] = NULL;
}

void proctitle_setting(int argc, char **argv)
{
    int       i;
    size_t    size;
    char     *title;
    char     *p;
        
    os_save_argv(argc, argv);
    
    os_init_proctitle();

    size = strlen(master_title);

    for (i = 0; i < argc; i++) {
        size += strlen(new_argv[i]) + 1;
    }

    title = os_calloc(size);

    p = title;

    memcpy(p, master_title, strlen(master_title));
    p += strlen(master_title);
    for (i = 0; i < argc; i++) {
        *p++ = ' ';
        strncpy(p, new_argv[i], strlen(new_argv[i]));
        p += strlen(new_argv[i]);
    }
    
    _proctitle_setting(title);
}

int main(int argc, char **argv)
{
    sigset_t  set;

    pthread_key_create(&thread_private_key, NULL);

    init_signals();

    os_parse_options(argc, argv);

    if (cf_daemon) {
        os_daemon();
    }

    // todo some other things

    proctitle_setting(argc, argv);

    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGTERM);
    //sigaddset(&set, SIGINT);
    sigaddset(&set, SIGPIPE);
    sigaddset(&set, SIGSYS);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);

    sigprocmask(SIG_BLOCK, &set, NULL);

    sigemptyset(&set);
    
    os_worker_start();
    
    for (;;) {
        sigsuspend(&set);
        printf("receive a signal\n");
        //if (reconfig) {
        //    event_notify_signal();
        //}
    }
    
    return 0;
}
