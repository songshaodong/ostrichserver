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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <memory.h>
#include <signal.h>
#include <core_log.h>
#include <event.h>
#include <evthread.h>
#include <assert.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <ctype.h>

#ifndef likely
#define likely(x)	__builtin_expect (!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)	__builtin_expect (!!(x), 0)
#endif

enum OS_ERRNO {
    OS_OK = 0,
    OS_ERR = -1

};

char *os_strdup(char *str);
char *os_strndup(char *str, size_t n);
ssize_t os_natoi(char *line, size_t n);
ssize_t os_atoi(char *line);
    
#endif
