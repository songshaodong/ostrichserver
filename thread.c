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
#include "thread.h"
#include "event.h"

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

void thread_process_event(evthread *t)
{
    externalq  *queue = &t->externalqueue;
    event      *e;

    printf("thread %p running\n", t);

    for (;;) {

        e = t->localqueue.dequeue();
        if (e) {
            t->process_event(e);
        }
        if (!atomic_list_empty(&queue->al)) {
            queue->dequeueall();
        }

        
    }
}

void local_schedule_imm(event *e, int eventtype)
{
    evthread *evt = current_thread(thread_private_key);

    if (eventtype == INIT_POLL) {
        e->cont->event_handler = netio_pollevent;
        evt->localqueue.enqueue(e);
    }
}

void thread_init(evthread *evt)
{
    pollbase  *pb = NULL;
    event     *e = NULL;
    
    pthread_setspecific(thread_private_key, evt);

    pb = pollbase_init(DEFAULT_EVENTLIST);

    if (pb == NULL) {
        return;
    }

    evt->eventbase = pb;

    e = os_calloc(sizeof(event));

    e->schedule_imm = local_schedule_imm;

    e->cont = (continuation *)pb;

    e->t = evt;

    e->schedule_imm(e, INIT_POLL);
}

void *thread_loop_internal(void *data)
{
    evthread *evt = data;

    thread_init(evt);
    
    while (1) {
        thread_process_event(evt);
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

void *make_threads_pool(int type, int num)
{
    if (type == DEDICATED) {
        return make_threadrt_pool(threadrt_loop_internal, num);
    } else if (type == REGULAR) {
        return make_thread_pool(thread_loop_internal, num);
    }
}

inline void thread_event_wakeup(evthread *thr)
{
    externalq *queue;

    queue = &thr->externalqueue;
    
    mutex_acquire(&queue->lock);
    
    cond_signal(&queue->might_have_data);
    
    mutex_release(&queue->lock);
}

void thread_localq_enqueue(void *item)
{
    evthread *t = current_thread(thread_private_key);

    locallnk *lnk = NULL;
    event    *elnk = item;

    lnk = t->localqueue.link;

    elnk->ln.next  = (event *)lnk;

    t->localqueue.link = elnk;
}

void *thread_localq_dequeue()
{
    evthread *t = current_thread(thread_private_key);
    
    locallnk *lnk = NULL;
    locallnk *next = NULL;

    lnk = t->localqueue.link;

    if (lnk == NULL) {
        return NULL;
    }

    next = getlnknext(lnk);

    t->localqueue.link = next;

    return lnk;
    
}

void thread_localq_init(localq *lq)
{
    lq->link = NULL;
    lq->enqueue = thread_localq_enqueue;
    lq->dequeue = thread_localq_dequeue;
}

void thread_event_process(event *e)
{
    evthread *t = current_thread(thread_private_key);
    
    e->cont->event_handler(e);
    // todo redo event

    if (e->redo) {
        t->localqueue.enqueue(e);
    }
}

int thread_event_handler_init(evthread *t)
{
    t->process_event = thread_event_process;
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
        
        thread_externalq_init(&t[i].externalqueue);
        thread_localq_init(&t[i].localqueue);

        thread_event_handler_init(&t[i]);
        
    }
    
    return t;
}

void thread_event_enqueue(void *item)
{
    int was_empty;

    event *e = item;

    evthread *thread = e->t;

    was_empty = (atomic_list_push(&thread->externalqueue.al, e) == NULL);

    if (was_empty == false) {
        //thread_event_wakeup(thread);
        return;
    }
    
    printf("thread is %p, target al is %p, put event: %p\n", thread, &thread->externalqueue.al, e);
    
    thread_event_wakeup(thread);
}

void thread_event_dequeue()
{
    event      *e;
    evthread   *t = current_thread(thread_private_key);
    externalq  *queue = &t->externalqueue;
        
    mutex_acquire(&queue->lock);
    
    while (atomic_list_empty(&queue->al)) {
        //cond_timewait(&queue->might_have_data, &queue->lock, &ts); //todo
        cond_wait(&queue->might_have_data, &queue->lock);
    }

    //printf("get event: %p\n", e);
    
    mutex_release(&queue->lock);

    t->localqueue.link = (locallnk *)atomic_list_popall(&queue->al);

    //printf("new connection, fd: %d\n", ((netconnection *)e->cont)->ci.fd);   
}

void thread_externalq_init(externalq *eq)
{
    event   e;

    mutex_init(&eq->lock);
    
    cond_init(&eq->might_have_data);
    
    atomic_list_init(&eq->al, "processor external queue", 
        (char *)&e.ln.next - (char *)&e);
    
    eq->enqueue = thread_event_enqueue;

    eq->dequeueall = thread_event_dequeue;
    
    eq->wakeup = thread_event_wakeup;
}

