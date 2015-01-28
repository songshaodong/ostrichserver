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

#ifndef __NETEVENT_H__
#define __NETEVENT_H__

#include <common.h>

#define  DEFAULT_EVENTLIST  1024

typedef struct {
    void         *eventbase;
    int           fd;
    int           event;
    uint32_t      active:1;
} netevent;

int netevent_create();
int netevent_init();

#endif