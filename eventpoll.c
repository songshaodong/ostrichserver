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
#include <eventpoll.h>
#include <netevent.h>

extern thread_key_t thread_private_key;

#define get_local_epbase() (((evthread *)current_thread(thread_private_key))->eventbase)

epbase mainepbase;

int epoll_init(int size)
{
    epbase *base = get_local_epbase();

    base->evsize = size;
    
    base->epfd = epoll_create(size);
    if (base->epfd < 0) {
        return OS_ERR;
    }
    
    base->evlist = os_calloc(sizeof(struct epoll_event) * base->evsize);
    if (base->evlist == NULL) {
        return OS_ERR;
    }
    
    base->eflags |= EPEDGE; // default using edge trigger.

    return OS_OK;
}

int epoll_event_start(netevent *ne, int flag)
{
    struct epoll_event ev;
    int                op;

    if (ne->active) {
        return OS_OK;
    }
    
    epbase *base = get_local_epbase();

    os_memset(&ev, 0, sizeof(ev));
    
    ev.events |= flag | EPOLLET;
    ev.data.ptr = ne;

    op = EPOLL_CTL_ADD;

    epoll_ctl(base->epfd, op, ne->fd, &ev);

    ne->active = 1;
    ne->event = flag;

    return OS_OK;
}

int epoll_event_modify(netevent *ne, int flag)
{
    int                op;
    int                event;
    struct epoll_event ev;

    epbase *base = get_local_epbase();
    
    if (!ne->active) {
        op = EPOLL_CTL_ADD;
        event = flag | EPEDGE;
    } else {
        op = EPOLL_CTL_MOD;
        event = ne->event;
        event |= flag;
    }

    ev.events = event;
    ev.data.ptr = ne;
    
    epoll_ctl(base->epfd, op, ne->fd, &ev);

    return OS_OK;
}
