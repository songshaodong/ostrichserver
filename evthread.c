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

#include <common.h>
#include <evthread.h>
#include <netevent.h>

evthread  *evthread_pool;
int        evthread_num;

thread_key_t thread_private_key;

int thread_local_init()
{
    netevent_init();

    return OS_OK;
}

void *thread_internal(void *data)
{
    evthread *evt = data;

    pthread_setspecific(thread_private_key, evt);

    thread_local_init();
    
    while (1) {
        sleep(1);
    }
    
    return NULL;
}

evthread *current_thread(thread_key_t key)
{
    return pthread_getspecific(key);
}

int _thread_create(evthread *evt, int stacksize, int detached)
{
    int ret;
    
    pthread_attr_t attr;

    pthread_attr_init(&attr);
  
    pthread_attr_setstacksize(&attr, stacksize);

    if (detached) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }
    
    ret = pthread_create(&evt->tid, &attr, evt->handler, evt);

    pthread_attr_destroy(&attr);

    return OS_OK;
}

int thread_create(evthread *evt, int stacksize, int detached, int type)
{
    if (type == REGULAR) {
        evt->handler = thread_internal;
    }

    _thread_create(evt, stacksize, detached);

    return OS_OK;
}

int threadpool_init()
{
    int n = DEFAULT_THREADS;
    int i = 0;

    pthread_key_create(&thread_private_key, NULL);

    evthread_pool = os_calloc(n * sizeof(evthread));
    
    for (i = 0; i < n; i++) {
        
        thread_create(evthread_pool + i, STACK_SIZE, 0, REGULAR);
        
        evthread_num++;
    }
    
    return OS_OK;
}

