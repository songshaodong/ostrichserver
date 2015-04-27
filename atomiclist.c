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

void atomic_list_init(atomiclist *al, char *name, int next_offset)
{
    al->name = name;
    al->offset = next_offset;
    SET_ATOMICLIST_POINTER_VERSION(al->head, (char *)0, 0);
}

void *atomic_list_push(atomiclist *al, void *data)
{
    head_p          head;
    head_p          temp;
    volatile void **addroffset = (volatile void **)((char *)data + al->offset);
    volatile void  *h = NULL;
    int             result = 0;

    do {
        head = al->head;
        h = ALPOINTER(head);
        *addroffset = h;
        SET_ATOMICLIST_POINTER_VERSION(temp, data, ALVERSION(head));
        result = atomic_cas((int64_t *)&al->head, head.data, temp.data);
    } while (result == 0);

    return (void *)h;
}

void *atomic_list_pop(atomiclist *al)
{
    head_p  temp;
    head_p  next;
    void   *ptr;

    int result = 0;

    do {
        
        temp = al->head;
        
        if ((void *)ALPOINTER(temp) == NULL) {
            return NULL;
        }
        
        SET_ATOMICLIST_POINTER_VERSION(next, 
            *((volatile void **)((char *)((void *)ALPOINTER(temp)) + al->offset)), 
            ALVERSION(temp) + 1);

        result = atomic_cas((int64_t *)&al->head.data, temp.data, next.data);
    } while (result == 0);

    ptr = (void *)ALPOINTER(temp);

    *((volatile void **)((char *)ptr + al->offset)) = NULL;

    return ptr;
}


inline bool atomic_list_empty(atomiclist *al)
{
    return !((void *)ALPOINTER(al->head));
}
