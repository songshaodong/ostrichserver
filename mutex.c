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

pthread_mutexattr_t  g_mutex_attr;

inline void mutex_init(mutex_t *mutex)
{
    pthread_mutexattr_init(&g_mutex_attr);
    pthread_mutexattr_setpshared(&g_mutex_attr, PTHREAD_PROCESS_SHARED);
    
}

inline void cond_init(cond_t *cond)
{
    pthread_cond_init(cond, NULL);
}

inline void mutex_acquire(mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
}

inline void mutex_destory(mutex_t *mutex)
{
    pthread_mutex_destroy(mutex);
}

inline void mutex_release(mutex_t *mutex)
{
    pthread_mutex_unlock(mutex);
}

inline void cond_wait(cond_t *cond, mutex_t *m)
{
    pthread_cond_wait(cond, m);
}

inline int cond_timewait(cond_t *cond, mutex_t *m, struct timespec *t)
{
    int error;
    
    while (EINTR == (error = pthread_cond_timedwait(cond, m, t)));

    printf("cond timewait timeout, err: %d\n", error);

    return error; 
}

inline void cond_signal(cond_t *cond)
{
    pthread_cond_signal(cond);
}

inline void cond_broadcast(cond_t *cond)
{
    pthread_cond_broadcast(cond);
}

