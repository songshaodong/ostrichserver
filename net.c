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
/* TODO: */

#include "common.h"
#include "net.h"
#include "eventpoll.h"
#include "http_request.h"

netconnection *init_connection(int fd, conninfo *ci)
{
    netconnection *nc;

    nc = os_calloc(sizeof(netconnection));
    nc->ci = *ci;
    nc->ci.fd = fd;
    nc->status = EVENT_STARTUP;
}

void netio_init(event *ev)
{
    netconnection *nc = (netconnection *)ev->cont;

	continuation *cont = (continuation*)nc;

	/* load http module */
	cont->event_handler = http_wait_request_handler;

	nc->c.read = ev;
	nc->c.write = NULL;
    nc->c.fd = nc->ci.fd;

    assert(nc->status == NEW_CONNECTION);

    pollevent_start(ev, poll_init_event());

    printf("register an event, thread:%p, pollfd: %d\n", ev->t, ev->t->eventbase->pollfd);
    
    nc->status = WAIT_FOR_READ;

	evprocessor.schedule(cont, EVENT_IDLE); 
}

void netio_pollevent(event *e)
{
    evthread   *evt = current_thread(thread_private_key);
    pollbase   *ep = (pollbase *)e->cont;
    int         i;
        
    ep->result = ep->eventpoll(ep->pollfd, ep->evlist, ep->pesize, 
        ep->timeout);

    if (ep->result) {
        printf("get event: %d, thread: %p, pollfd: %d\n", ep->result, evt, 
            ep->pollfd);
    }
}


