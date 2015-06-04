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
#include "reconfig.h"

int reconfig_listenfd;


/*
int sock;

sock = socket(AF_UNIX, SOCK_STREAM, 0);
struct sockaddr_un saddr;
saddr.sun_family = AF_UNIX;
strcpy(saddr.sun_path, "reconfig_socket");

connect(sock, (struct sockaddr *)&saddr, sizeof(saddr));
*/
void *reconfig_main_thread(void *data)
{
    int connfd;

    // todo select.
    
    while (1) {
        connfd = accept(reconfig_listenfd, NULL, NULL);
        if (connfd < 0) {
            if (errno == EINTR) {
                continue;
            }
        }
    }
}

int reconfig_sock_init()
{
    reconfig_listenfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un saddr;

    memset(&saddr, 0, sizeof(saddr));
    
    saddr.sun_family = AF_UNIX;
    
    strcpy(saddr.sun_path, "reconfig_socket");

    unlink("reconfig_socket"); // todo

    bind(reconfig_listenfd, (struct sockaddr *)&saddr, sizeof(saddr));

    listen(reconfig_listenfd, 100);

    return OS_OK;
}

int reconfig_thread_init()
{ 
    reconfig_sock_init();
    
    make_thread(reconfig_main_thread);

    return OS_OK;
}
