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
#ifndef _THREAD_H_
#define _THREAD_H_

#include "common.h"

#define DEFAULT_THREADS  4

#define STACK_SIZE  (4 * 1024 * 1024)
#define thread_key_t pthread_key_t 
#define thread_t     pthread_t

struct thread {
    int             type;
    thread_key_t    private_key;
    thread_t        tid;
    void           *eventbase;
    threadproc      execute;
    externalq       externalqueue;
    evhandler       process_event;
};

struct thread_runtime {
    event    *static_event;
    evthread  thread;
};

enum THREAD_TYPE {
    REGULAR = 0,
    DEDICATED
};

/*enum THREAD_TYPE {
    ACCEPTOR,
    NET,
    DISK,
    LOG
}; */


int thread_create(threadrt *evt, int stacksize, int detached);
threadrt *make_thread_pool(threadproc exec, int evtype, int num);
void *thread_loop_internal(void *data);
evthread *current_thread(thread_key_t key);

#endif
