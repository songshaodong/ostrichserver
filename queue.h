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

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "common.h"

// <5ms, 10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120
#define N_PQ_LIST        10
#define IDX_TIME(t) (5 << t)

typedef struct {
    int      last_check_index;
    int      last_check_time;
    void    *buckets[N_PQ_LIST]; 
} priority_queue;

inline void priority_queue_init();
void priority_enqueue(event *e, time_t now);
void priority_queue_check(priority_queue *queue, time_t now);

#endif