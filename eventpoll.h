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

#ifndef _EVENTPOLL_H_
#define _EVENTPOLL_H_

#include "common.h"

enum EPTRIGGER {
    EPLEVEL = 0x00000001,
    EPEDGE = 0x00000002
};

#define  EPREAD     EPOLLIN
#define  EPWRITE    EPOLLOUT
#define  EPTIMEOUT          5      // ms

typedef struct {
    int                 epfd;
    int                 evsize;
    int                 eflags;
    struct epoll_event *evlist;
} epbase;

epbase *epoll_init(int size);
int epoll_event_start(event *e, int flag);

#endif

