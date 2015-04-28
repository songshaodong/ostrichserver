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
#include "eventpoll.h"

extern thread_key_t thread_private_key;

#define get_local_epbase() (((evthread *)current_thread(thread_private_key))->eventbase)

epbase mainepbase;

epbase *epoll_init(int size)
{
    epbase *base = os_calloc(sizeof(epbase));

    base->evsize = size;
    
    base->epfd = epoll_create(size);
    if (base->epfd < 0) {
        return NULL;
    }
    
    base->evlist = os_calloc(sizeof(struct epoll_event) * base->evsize);
    if (base->evlist == NULL) {
        return NULL;
    }
    
    base->eflags |= EPEDGE; // default using edge trigger.

    return base;
}

int epoll_event_start(event *e, int flag)
{
    struct epoll_event ev;
    int                op; 
    netconnection     *nc;

    if (e->active) {
        return OS_OK;
    }
    
    epbase *base = get_local_epbase();

    os_memset(&ev, 0, sizeof(ev));
    
    ev.events |= flag | EPOLLET;
    ev.data.ptr = e;

    op = EPOLL_CTL_ADD;

    nc = (netconnection *)e->cont;

    epoll_ctl(base->epfd, op, nc->ci.fd, &ev);

    e->active = 1;
    e->flag = flag;

    return OS_OK;
}

int epoll_event_modify(event *e, int flag)
{
    int                op;
    int                event;
    struct epoll_event ev;
    netconnection     *nc;

    epbase *base = get_local_epbase();
    
    if (!e->active) {
        op = EPOLL_CTL_ADD;
        event = flag | EPEDGE;
    } else {
        op = EPOLL_CTL_MOD;
        event = e->flag;
        event |= flag;
    }

    ev.events = event;
    ev.data.ptr = e;

    nc = (netconnection *)e->cont;
    
    epoll_ctl(base->epfd, op, nc->ci.fd, &ev);

    return OS_OK;
}

