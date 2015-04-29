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
#include "mutex.h"
#include "atomiclist.h"

#define DEFAULT_THREADS  3

#define STACK_SIZE  (4 * 1024)
#define thread_key_t pthread_key_t 
#define thread_t     pthread_t

#define locallnk  event  // todo
#define getlnknext(link) (link->ln.next) //todo 

struct localqueue {
    locallnk *link;
    void    (*enqueue)(void *item);
    void   *(*dequeue)();
};

struct external_queue {
    mutex_t       lock;
    cond_t        might_have_data;
    atomiclist    al;
    void        (*enqueue)(void *item);
    void        (*wakeup)(evthread *evt);
    void        (*dequeueall)();
};

enum THREAD_TYPE {
    REGULAR = 0,
    DEDICATED
};

struct thread {
    int             type;
    thread_key_t    private_key;
    thread_t        tid;
    pollbase       *eventbase;
    threadproc      execute;
    externalq       externalqueue;
    localq          localqueue;
    void          (*process_event)(event *e);
};

struct thread_runtime {
    event    *static_event;
    evthread  thread;
};

int thread_create(void *t, int type, int stacksize, int detached);
evthread *make_thread_pool(threadproc exec, int num);
threadrt *make_threadrt_pool(threadproc exec, int num);
void *threadrt_loop_internal(void *data);
void *thread_loop_internal(void *data);
void *make_threads_pool(int type, int num);
evthread *current_thread(thread_key_t key);
void eventprocessor_init();
void thread_externalq_init(externalq *eq);
int thread_event_handler_init();

#endif
