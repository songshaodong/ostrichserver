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

#ifndef _TIMER_H_
#define _TIMER_H_

#include "common.h"

typedef struct timespec hrtime_t;

#define get_sec_time(ht)  (ht.tv_sec)
#define get_msec_time(ht) (ht.tv_sec * 1000 + ht.tv_nsec / 1000 / 1000)
#define get_usec_time(ht) (ht.tv_sec * 1000 * 1000 + ht.tv_nsec / 1000)
#define get_nsec_time(ht) (ht.tv_sec * 1000 * 1000 * 1000 + ht.tv_nsec)

hrtime_t get_current_time();

#define add_timer event_add_timer
#define del_timer event_del_timer

#endif