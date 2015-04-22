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
#include "acceptor.h"

processor acceptor;


int accept_block_loop(void *data)
{
    event *e = data;

    struct sockaddr *addr;
    socklen_t *addrlen;

    int fd = e->fd;
    int connfd;

    while (1) {
        fd = accept(fd, addr, addrlen);
    }
}

int acceptor_init()
{
    threadrt     *rtpool = NULL;
    continuation *c = NULL;
    int           i;

    // todo accept threads configed
	rtpool = make_thread_pool(thread_loop_internal, EPEDGE, 
	    MAX_ACCEPTOR_THREADS);

    if (rtpool == NULL) {
        return OS_ERR;
    }

    for (i = 0; i < MAX_ACCEPTOR_THREADS; i++) {
        c = os_calloc(sizeof(continuation));
        c->event_handler = accept_block_loop;
        rtpool[i].static_event->cont = c;
        thread_create(rtpool + i, STACK_SIZE, 1);
    }
    
    acceptor.workerpool = rtpool;
}
