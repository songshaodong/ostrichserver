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

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "common.h"

struct thread_processor {
};

struct external_queue {
    mutex_t      mutex;
    cond_t       cond;
    atomiclist   al;
    void        (*enqueue)(atomiclist *al, void *item);
    void       *(*dequeue)();
};

typedef struct {
    processor    base;
    void       (*schedule_imm)(continuation *cont, int eventtype);
    event     *(*assign_thread)();
    threadrt    *eventthread; // todo support other type threads
    int          n_threads;
    int          next_thread;
} _eventprocessor;


void eventprocessor_init();
void eventprocessor_externalq_init(externalq *eq);

#endif