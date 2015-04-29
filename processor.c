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
#include "processor.h"


_eventprocessor  evprocessor;

void event_schedule(continuation *cont, int eventtype)
{
    event    *ev;
    evthread *thread;
    
    ev = os_calloc(sizeof(event));
    ev->type = eventtype;
    ev->cont = cont;

    thread = evprocessor.assign_thread();

    printf("schedule accept event to thread: %p\n", thread);

    ev->t = thread;
    //ev->mutexlock = NULL; // todo init mutex.
    ev->t->externalqueue.enqueue(ev);
}

evthread *event_assign_thread(int eventtype)
{
    int              next;
    evthread        *t;
    _eventprocessor *evp = &evprocessor;
    
    (void) eventtype; // todo other types supported.

    next = evp->next_thread++ % evp->n_threads;

    t = evp->eventthread;

    return &t[next];
}

void eventprocessor_init(int n_threads)
{
    int       i;
    evthread *t;
    
    evprocessor.schedule = event_schedule;
    
    evprocessor.assign_thread = event_assign_thread;

    t = make_threads_pool(REGULAR, n_threads);

    evprocessor.n_threads = n_threads;
    
    evprocessor.next_thread = 0;

    for (i = 0; i < n_threads; i++) {
        thread_create(t + i, REGULAR, STACK_SIZE, 1);
    }

    evprocessor.eventthread = t;
}
