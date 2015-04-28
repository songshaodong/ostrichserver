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
#include "config.h"
#include "acceptor.h"
#include "server.h"
#include "event.h"
#include "processor.h"

extern _eventprocessor evprocessor;
_eventprocessor acceptor;

int do_accept_loop = 1;

int handle_accept(event *ev) 
{
    continuation  *cont = ev->cont;

    accept_block_loop(cont);

    return OS_OK;
}

int accept_block_loop(continuation *cont)
{
    int            fd = -1;
    event         *ev;
    tcp_acceptor  *ta;
    conninfo       ci;
    netconnection *nc;
    continuation  *c;

    ta = (tcp_acceptor *)cont;

    memset(&ci, 0, sizeof(conninfo));

    do {

        fd = accept(ta->serverfd, &ci.cliaddr, &ci.cliaddrlen);

        printf("accept fd: %d\n", fd);
        
        nc = init_connection(fd, &ci);
        
        c = (continuation *)nc;
        
        c->event_handler = netio_init; 
        
        evprocessor.schedule_imm(c, EVENT_STARTUP);
        
    } while (do_accept_loop);
}

// todo support other protocol.
int acceptor_init()
{
    int            i;
    int            listenfd;
    event         *staticevent;
    threadrt      *rtpool = NULL;
    continuation  *c = NULL;
    tcp_acceptor  *netacceptor;
    netconnection *conn;
    record        *rc;
    int            port = DEF_SERVER_PORT;

    rc = get_config_record("net.tcp.listen");

    if (rc) {
        port = rc->data.config_int;
    }
    
    listenfd = protocol_listen_open(AF_INET, SOCK_STREAM, 0, NULL, port);
    
    netacceptor = os_calloc(sizeof(tcp_acceptor));
    
    conn = os_calloc(sizeof(netconnection));
    
    netacceptor->serverfd = listenfd;
    
    netacceptor->cont.event_handler = handle_accept;
    
    // todo accept threads configed
    rtpool = make_threads_pool(DEDICATED, MAX_ACCEPTOR_THREADS);

    if (rtpool == NULL) {
        return OS_ERR;
    }

    acceptor.eventthread = rtpool;
    acceptor.n_threads = MAX_ACCEPTOR_THREADS;
    acceptor.next_thread = 0;
    
    for (i = 0; i < MAX_ACCEPTOR_THREADS; i++) {

        staticevent = rtpool[i].static_event;
        staticevent->cont = &netacceptor->cont;
        staticevent->t = &rtpool[i].thread;
        staticevent->type = ACCEPTEVENT;
        
        thread_create(&rtpool[i], DEDICATED, STACK_SIZE, 1);
    }
    
}

