/* 
   Copyright (c) 2014-2015 flashbuckets 

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

#ifndef __EVTHREAD_H__
#define __EVTHREAD_H__

#include <common.h>

#define DEFAULT_THREADS  4

#define STACK_SIZE  (4 * 1024 * 1024)
#define thread_key_t pthread_key_t 
#define thread_t     pthread_t

enum THREAD_TYPE {
    REGULAR,
    DEDICATED
};

typedef void *(*threadhanlder)(void *);

typedef struct {
    int             type;
    thread_key_t    private_key;
    thread_t        tid;
    void           *eventbase;
    event_engine   *evengine;
    threadhanlder   handler;
} evthread;

evthread *current_thread(thread_key_t key);
int threadpool_init();
int thread_create(evthread *evt, int stacksize, int detached, int type);
int thread_local_init();

#endif