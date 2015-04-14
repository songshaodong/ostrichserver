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

#include <event.h>
#include <pthread.h>

typedef struct thread evthread;
typedef struct thread_processor processor;
typedef struct external_queue   externalq;

struct external_queue {
} externalq;

struct thread_processor {
    externalq   pushqueue;
    evhandler   process_event;
} processor;

struct thread {
    int             type;
    thread_key_t    private_key;
    thread_t        tid;
    void         *(*execute)(void *);
};

int thread_create(evthread *evt, int stacksize, int detached);

#endif
