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
#include "signals.h"

void sig_term(int signum)
{
    pid_t   pid = 0;
    int     status = 0;
    
    killpg(0, signum);

    for (;;) {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid <= 0) {
            break;
        }

        workerid = pid;
        workerstatus = status;
    }

    exit(0);
}

void sig_child(int signum)
{
    pid_t pid = 0;
    int   status = 0;

    for (;;) {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid <= 0) {
            break;
        }

        workerid = pid;
        workerstatus = status;
    }
}


void sig_reconfig(int signum)
{
    reconfig = 1;
}

void sig_restart(int signum)
{
    restart = 1;
}

int init_signals()
{
    struct sigaction action;

    action.sa_handler = sig_term;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);

    action.sa_handler = sig_child;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGCHLD, &action, NULL);

    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGPIPE, &action, NULL);
    sigaction(SIGSYS, &action, NULL);

    action.sa_handler = sig_reconfig;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGUSR1, &action, NULL);

    action.sa_handler = sig_restart;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGUSR2, &action, NULL);

    return OS_OK;
}


