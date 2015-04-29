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
#include "net.h"
#include "eventpoll.h"

extern thread_key_t  thread_private_key;

netconnection *init_connection(int fd, conninfo *ci)
{
    netconnection *nc;

    nc = os_calloc(sizeof(netconnection));
    nc->ci = *ci;
    nc->ci.fd = fd;
}

int netio_init(event *ev)
{
    assert(ev->type == NEW_CONNECTION);

    pollevent_start(ev, poll_init_event());
    
    ev->type = WAIT_FOR_READ;
    
    return OS_OK;
}

int netio_pollevent(event *e)
{
    evthread   *evt = current_thread(thread_private_key);
    pollbase   *ep = (pollbase *)e->cont;
    int         i;

    ep->result = ep->eventpoll(ep->pollfd, ep->evlist, ep->pesize, ep->timeout);

    return OS_OK;
}

