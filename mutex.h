
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

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "common.h"

#define mutex_t      pthread_mutex_t
#define cond_t       pthread_cond_t

inline void mutex_init(mutex_t *mutex);
inline void cond_init(cond_t *cond);
inline void mutex_acquire(mutex_t *mutex);
inline void mutex_release(mutex_t *mutex);
inline void cond_wait(cond_t *cond, mutex_t *m);
inline int cond_timewait(cond_t *cond, mutex_t *m, struct timespec *time);
inline void cond_signal(cond_t *cond);
inline void cond_broadcast(cond_t *cond);


#endif
