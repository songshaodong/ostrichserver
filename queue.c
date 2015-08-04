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
#include "queue.h"
#include "event.h"

__thread priority_queue event_priority_queue;

inline void priority_queue_init(priority_queue *pq)
{
    pq->last_check_index = 0;
    pq->last_check_time = 0;
    
    memset(&pq->buckets, 0, sizeof(void *) * N_PQ_LIST);
}

void event_priority_enqueue(event *e, int64_t now)
{
    time_t    t = e->timeout - now;
    int       i = 0;
    event    *head;
    
    if (t <= IDX_TIME(3))
    {
      if (t <= IDX_TIME(1)) {
        if (t <= IDX_TIME(0)) {
          i = 0;
        } else
        {
          i = 1;
        }
      } else {
        if (t <= IDX_TIME(2)) {
          i = 2;
        } else {
          i = 3;
        }
      }
    } else {
      if (t <= IDX_TIME(7)) {
        if (t <= IDX_TIME(5)) {
          if (t <= IDX_TIME(4)) {
            i = 4;
          } else {
            i = 5;
          }
        } else {
          if (t <= IDX_TIME(6)) {
            i = 6;
          } else {
            i = 7;
          }
        }
      } else {
        if (t <= IDX_TIME(8)) {
          i = 8;
        } else {
          i = 9;
        }
      }
    }

    head = (event *)event_priority_queue.buckets[i];
    if (head) {
        getlnknext(e) = head;
        ((event *)event_priority_queue.buckets[i])->ln.next = e;
    } else {
        event_priority_queue.buckets[i] = e;
    }
}

void priority_queue_check(priority_queue *queue, int64_t now)
{
    (void) queue;
    
    int i, j, k = 0;
    event      *e;
    event      *next;
    event      *head;
    
    uint32_t check_buckets = (uint32_t) (now / IDX_TIME(0));
    
    uint32_t todo_buckets = check_buckets ^ event_priority_queue.last_check_index;
    
    event_priority_queue.last_check_time = now;
    
    event_priority_queue.last_check_index = check_buckets;
    
    todo_buckets &= ((1 << (N_PQ_LIST - 1)) - 1);
    
    while (todo_buckets) {
        k++;
        todo_buckets >>= 1;
    }
    
    for (i = 1; i <= k; i++) {
        
        e = event_priority_queue.buckets[i];
        
        event_priority_queue.buckets[i] = NULL;
    
        while (e != NULL) {

            time_t tt = ((event *)e)->timeout - now;
            
            for (j = i; j > 0 && tt <= IDX_TIME(j - 1);) {
              j--;
            }

            head = event_priority_queue.buckets[j];
            if (head) {
                getlnknext(e)= head;
                ((event *)event_priority_queue.buckets[i])->ln.next = e;
            } else {
                event_priority_queue.buckets[j] = e;
            }

            e = getlnknext(e);
        }
    }
}

inline void priority_queue_process_ready(priority_queue *queue, int64_t  now)
{
    (void) queue;
    
    event    *e = NULL;
    event    *head = (event *)event_priority_queue.buckets[0];
    evthread *t = current_thread(thread_private_key);

    e = head;
    
    while (e) {
        e = getlnknext(e);
        t->process_event(e);
    }
}
