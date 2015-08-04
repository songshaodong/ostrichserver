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

void
os_strlow(u_char *dst, u_char *src, size_t n)
{
    while (n) {
        *dst = os_tolower(*src);
        dst++;
        src++;
        n--;
    }
}

os_uint_t
os_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    os_uint_t  c1, c2;

    while (n) {
        c1 = (os_uint_t) *s1++;
        c2 = (os_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {

            if (c1) {
                n--;
                continue;
            }

            return 0;
        }

        return c1 - c2;
    }

    return 0;
}

u_char *
os_strstrn(u_char *s1, char *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    do {
        do {
            c1 = *s1++;

            if (c1 == 0) {
                return NULL;
            }

        } while (c1 != c2);

    } while (os_strncmp(s1, (u_char *) s2, n) != 0);

    return --s1;
}


u_char *
os_strcasestrn(u_char *s1, char *s2, size_t n)
{
    os_uint_t  c1, c2;

    c2 = (os_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    do {
        do {
            c1 = (os_uint_t) *s1++;

            if (c1 == 0) {
                return NULL;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } while (c1 != c2);

    } while (os_strncasecmp(s1, (u_char *) s2, n) != 0);

    return --s1;
}



#if (OS_MEMCPY_LIMIT)

void *
os_memcpy(void *dst, const void *src, size_t n)
{
    if (n > OS_MEMCPY_LIMIT) {
		printf(RED "memcpy %uz bytes" GRAY, n);
    }
    return memcpy(dst, src, n);
}

#endif


