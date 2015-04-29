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

inline continuation *event_init(event *e)
{
    continuation *cont;

    cont = os_calloc(sizeof(continuation));

    e->cont = cont;
}

pollbase *pollbase_init(int size)
{
    pollbase *base = os_calloc(sizeof(pollbase));

    base->pesize = size;
    
    base->pollfd = pollfd_create(size);
    if (base->pollfd < 0) {
        return NULL;
    }
    
    base->evlist = os_calloc(sizeof(pelist) * base->pesize);
    if (base->evlist == NULL) {
        return NULL;
    }
    
    //base->eflags |= pe_get_flags(); // default using edge trigger.

    base->eventpoll = pollevent_handle;

    return base;

}

