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
#include "notify.h"

int event_fd;
int notify_epfd;

int event_notify_init()
{
    struct epoll_event ev;

    event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    
    ev.events = EPOLLIN;

    ev.data.fd = event_fd;
    
    notify_epfd = epoll_create(1);

    epoll_ctl(notify_epfd, EPOLL_CTL_ADD, event_fd, &ev);

    return OS_OK;
}

int event_notify_signal()
{
    uint64_t  data = 1;
    
    write(event_fd, &data, sizeof(data));

    return OS_OK;
}

int event_notify_wait()
{
    int       result;
    int       nevent;
    uint64_t  data = 0;
    struct epoll_event ev;

    do {
        nevent = epoll_wait(notify_epfd, &ev, 1, -1);
    } while (nevent < 0 && errno == EINTR);

    if (nevent < 0) {
        return OS_ERR;
    }

    result = read(event_fd, &data, sizeof(data));

    if (result != sizeof(uint64_t)) {
        return OS_ERR;
    }

    
    return OS_OK;
}
