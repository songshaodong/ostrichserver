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
int isdaemon = 0;
int event_fd = -1;

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
                case 'q':
                    quiet = 1;
                    break;
                case 'r':
                    reconfig = 1;
                    break;
                case 's':
                    restart = 1;
                case 't':
                    isdaemon = 0;
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

    if (isdaemon) {    
        rc = fork();
        if (rc > 0) {
            return OS_OK;
        }
    }
    
    result = config_parse_file("record.config");

    eventprocessor_init(DEFAULT_THREADS);

    application_protocol_init("HTTP");
    
    acceptor_init();

    for (;;) {
        event_notify_wait(); // todo process errno
    }
}

int main(int argc, char **argv)
{

    pthread_key_create(&thread_private_key, NULL);

    init_signals();

    os_parse_options(argc, argv);

    if (isdaemon) {
        os_daemon();
    }

    // todo some other things

    os_worker_start();
    
    while (1) {
        sleep(1);
    }
    
    return 0;
}