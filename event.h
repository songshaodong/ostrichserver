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


enum  {
    ACCEPTEVENT = 0,
    NEW_CONNECTION = 0x0001,
    READ = 0x0002,
    WRITE = 0x0004,
    DISKIOEVENT = 0x0008
};

#define REGULAR_ET  (NEW_CONNECTION | READ | WRITE)

struct evtype {
    type_handler  set; 
};

typedef struct {
    //event *prev;
    event *next;
} evlink;

struct thread_event {
    int           type;
    int           active;
    int           flag;
    evthread     *t;
    //mutex_t      *mutexlock;
    continuation *cont;
    evlink        ln;
};

#endif
