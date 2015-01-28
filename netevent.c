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
#include <netevent.h>
#include <eventpoll.h>

extern event_engine main_event_engine;

int netevent_create()
{
    netevent *ne;

    ne = os_calloc(sizeof(netevent));
    if (ne == NULL) {
        return OS_ERR;
    }

    ne->eventbase = &main_event_engine;
    ne->fd = -1;

    return OS_OK;
}

int netevent_init()
{
    epoll_init(DEFAULT_EVENTLIST);

    return OS_OK;
}