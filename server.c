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
#include "server.h"

int protocol_listen_open(int domain, int type, int protocal, char *ipstr, int port)
{
    int                 listenfd;
    int                 servport = DEF_SERVER_PORT;
    uint32_t            ip = INADDR_ANY;
    struct sockaddr_in  servaddr;
    struct sockaddr_in  cliaddr;

    listenfd = socket(domain, type, protocal);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = domain;
    
    if (port > 0) {
        servport = port;
    }
    
    servaddr.sin_port = htons(servport);
    
    if (ipstr) {
        ip = inet_network(ipstr);
    }
    
    servaddr.sin_addr.s_addr = htons(ip);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    listen(listenfd, DEF_LSTEN_BACKLOG);

    // todo set recvbuf and sendbuf
    //fcntl(listenfd, F_SETFL, fcntl(listenfd, F_GETFL) | O_NONBLOCK);

    return listenfd;
}
