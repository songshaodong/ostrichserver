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

int os_daemon()
{
    int fd = fork();
    
    if (fd < 0) {
        return OS_ERR;
    }

    if (fd > 0) {
        exit(0);
    }

    masterid = getpid();

    if(setsid() == -1) {
        return OS_ERR;
    }

    umask(0);

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        return OS_ERR;
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);

    return OS_OK;
}

