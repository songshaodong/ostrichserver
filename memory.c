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
#include "memory.h"
#include <string.h>

void *os_malloc(size_t size)
{
    void *ptr;
    
    ptr = malloc(size);
    
    return ptr;
}

void *os_realloc(void *p, size_t size)
{
    void *ptr;
    
    ptr = realloc(p, size);
    
    return ptr;
}


void *os_calloc(size_t size)
{
    void *ptr;
    
    ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    
    return ptr;
}

void *os_memset(void *s, int c, size_t n)
{
    assert(s != NULL);

    return memset(s, c, n);
}

void os_free(void *ptr)
{
    free(ptr);
}
