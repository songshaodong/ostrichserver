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

int
os_recv(connection *c, u_char *buf, int size)
{
    int       n;
    int     err;
    event  *rev;

    rev = c->read;

    do {
        n = recv(c->fd, buf, size, 0);
        err = os_socket_errno;

		printf("recv: fd:%d %d of %d \n", c->fd, n, size);

        if (n == 0) {
            return n;

        } else if (n > 0) {

			c->receive += n;
            return n;
        }

        if (err == EAGAIN || err == EINTR) {

			printf(GREEN "recv() not ready, fd: %d, return:%d, sent: %d, \n" GRAY, 
                c->fd, n, size);
            n = OS_AGAIN;

        } else {
        	printf(RED "recv() failed: %s \n" GRAY, strerror(err));
            break;
        }

    } while (err == EINTR);

    if (n == OS_ERR) {
        rev->error = 1;
    }

    return n;
}

