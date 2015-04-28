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

#ifndef _NET_H_
#define _NET_H_

#include "common.h"
#include "connection.h"

enum  {
    INIT_POLL,
    ACCEPTEVENT,
    NEW_CONNECTION,
    WAIT_FOR_READ,
    READ_DATA,
    WAIT_FOR_WRITE,
    WRITE_DATA,
};

#define EVENT_STARTUP  (NEW_CONNECTION)

typedef struct {
    struct sockaddr  cliaddr;
    socklen_t        cliaddrlen;
    int              fd;
} conninfo;

typedef struct {
    connection       c;
    conninfo         ci;
    int              status;
} netconnection;

netconnection *init_connection(int fd, conninfo *ci);
int netio_init(event *ev);
int netio_pollevent(event *e);

#endif