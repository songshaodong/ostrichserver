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

#include <common.h>
#include <thread.h>

thread_key_t thread_private_key;

evthread *current_thread(thread_key_t key)
{
    return pthread_getspecific(key);
}

void *threadrt_loop_internal(void *data)
{
    threadrt   *rt = data;
    evthread   *evt = &rt->thread;
    event      *e = rt->static_event;

    pthread_setspecific(thread_private_key, evt);

    if (e) {
        e->cont->event_handler(e);
    }

    return NULL;
}

void *thread_loop_internal(void *data)
{
    evthread   *rt = data;
    externalq  *queue = &rt->externalqueue;
    atomiclist *l = &queue->al;
    struct timespec    ts;

    pthread_setspecific(thread_private_key, rt);

    ts.tv_sec = 3;
    ts.tv_nsec = 0;

    printf("thread %p running\n", rt);
    
    while (1) {
        while (atomic_list_empty(l)) {
            cond_timewait(&queue->might_have_data, &queue->lock, &ts);
        }

        printf("new connection\n");
    }

    return NULL;
}


int thread_create(void *thr, int type, int stacksize, int detached)
{
    int       ret;

    evthread *t = thr; 

    pthread_attr_t attr;

    if (type == DEDICATED) {
        t = &((threadrt *)thr)->thread;
    }    

    pthread_attr_init(&attr);
  
    pthread_attr_setstacksize(&attr, stacksize);

    if (detached) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }
    
    ret = pthread_create(&t->tid, &attr, t->execute, thr);
    if (ret < 0) {
        return OS_ERR;
    }

    pthread_attr_destroy(&attr);

    return OS_OK;
}

threadrt *make_threadrt_pool(threadproc exec, int num)
{
    threadrt *rt;
    evthread *t;
    int       i;
    event    *e;
    
    rt = os_calloc(num * sizeof(threadrt));
    if (rt == NULL) {
        return NULL;
    }

    for (i = 0; i < num; i++) {
        
        rt[i].thread.execute = exec;
        rt[i].thread.type = DEDICATED;

        e = os_malloc(sizeof(event));
        e->t = &rt[i].thread;
        rt[i].static_event = e;
    }
    
    return rt;
}

evthread *make_thread_pool(threadproc exec, int num)
{
    evthread *t;
    int       i;
    event    *e;
    
    t = os_calloc(num * sizeof(evthread));
    if (t == NULL) {
        return NULL;
    }

    for (i = 0; i < num; i++) {
        
        t[i].execute = exec;
        t[i].type = REGULAR;
        
        eventprocessor_externalq_init(&t[i].externalqueue);
    }
    
    return t;
}

void *make_threads_pool(int type, int num)
{
    if (type == DEDICATED) {
        return make_threadrt_pool(threadrt_loop_internal, num);
    } else if (type == REGULAR) {
        return make_thread_pool(thread_loop_internal, num);
    }
}

