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
#include <stdlib.h>
#include <string.h>

char *os_strdup(char *str)
{
    size_t  size;
    char   *ptr;

    size = strlen(str);
    
    ptr = os_malloc(size + 1);

    memcpy(ptr, str, size);

    ptr[size] = '\0';
    
    return ptr; 
}

char *os_strndup(char *str, size_t n)
{
    char *ptr;

    ptr = os_malloc(n + 1);

    memcpy(ptr, str, n);

    ptr[n] = '\0';
    
    return ptr;
}

ssize_t
os_natoi(char *line, size_t n)
{
    ssize_t  value;

    if (n == 0) {
        return OS_ERR;
    }

    for (value = 0; n--; line++) {
        if (*line < '0' || *line > '9') {
            return OS_ERR;
        }

        value = value * 10 + (*line - '0');
    }

    if (value < 0) {
        return OS_ERR;
    } else {
        return value;
    }
}

ssize_t os_atoi(char *line)
{
    size_t   n = strlen(line);

    return os_natoi(line, n);
}

