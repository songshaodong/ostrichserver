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

#ifndef _EVENT_H_
#define _EVENT_H_

#include "common.h"
#include "eventpoll.h"
#include "continuation.h"
#include "event.h"


#define  DEFAULT_EVENTLIST  32768

#define   pollfd_create(size)   epoll_create(size)
#define   pelist                struct epoll_event
#define   pollevent_handle      epoll_wait
#define   pollevent_start(e, type)  epoll_event_start(e, type)
#define   poll_init_event()       (EPOLLIN | EPOLLOUT)
#define   poll_timeout()          EPTIMEOUT


#define  EVENT_IMM   0x00000000
#define  EVENT_IDLE  0x00000001
#define  EVENT_REDO  0x00000002


#define qlink  event  // todo
#define getlnknext(link) (link->ln.next) //todo 


struct evtype {
    type_handler  set; 
};

struct event_list {
    event *next;
};

struct thread_event {
    evlink        ln;
    int           type;
    int           active;
    int           flag;
    int           redo;
    int64_t       timeout;
    evthread     *t;
    continuation *cont;
    void        (*schedule)(event *e, int eventtype);
	unsigned      timer_set:1;
	unsigned	  timedout:1;
	unsigned 	  error:1;
};

typedef struct {
    continuation         cont;
    int                  pollfd;
    int                  pesize;
    int                  timeout;
    pelist              *evlist;
    int                (*eventpoll)(int fd, pelist *l, int size, int tm);
    int                  result;
} pollbase;

inline continuation *event_init(event *e);
pollbase  *pollbase_init(int size);

static inline void
add_timer(event *ev, msec ter)
{
	
}

static inline void
del_timer(event *ev, msec ter)
{
	
}

#endif
