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
#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include "common.h"

typedef int64_t atomic_int;
typedef uint64_t atomic_uint;
typedef volatile atomic_uint atomic_t;

/* old "as" does not support "pause" opcode */
#define cpu_pause()         __asm__ (".byte 0xf3, 0x90")

void spin_lock(atomic_t *lock, atomic_int value);

#define _lock(lock)	    spin_lock(lock, 1)
#define _trylock(lock)  (*(lock) == 0 && atomic_cas(lock, 0, 1))
#define _unlock(lock)    *(lock) = 0


#define atomic_cas(val, old, new)   \
    __sync_bool_compare_and_swap(val, old, new)


#define atomic_fetch_add(value, add)  \
    __sync_fetch_and_add(value, add)

#endif