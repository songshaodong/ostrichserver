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

void atomic_list_push(atomiclist *al, void *data)
{
}

void *atomic_list_pop(atomiclist *al)
{
}

