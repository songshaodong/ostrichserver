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

#ifndef _ATOMICLIST_H_
#define _ATOMICLIST_H_

#include "common.h"

#define HAS_128BIT_CAS 1   // todo test 128bit cas support.

typedef union
{
    
#if (defined(__i386__) && (SIZEOF_VOIDP == 4))

    struct {
      void *pointer;
      int32_t version;
    } s;
    int64_t data;
    
#elif defined(__x86_64__) || defined(__ia64__)

    int64_t data;

#endif

} head_p;
  
typedef struct
{
    volatile head_p head;
    const char     *name;
    uint32_t        offset;
} atomiclist;

extern inline void __LD64(void *dst, void *src);

#if defined(__x86_64__) || defined(__ia64__)
#define _LD64(dst,src) *((uint64_t*)&(dst)) = *((uint64_t*)&(src))
#else
#define _LD64(dst,src) (__LD64((void *)&(dst), (void *)&(src)))
#endif


#if (defined(__i386__) || (SIZEOF_VOIDP == 4))

#define ATOMICLIST_POINTER(_x) (_x).s.pointer
#define ATOMICLIST_VERSION(_x) (_x).s.version
#define SET_ATOMICLIST_POINTER_VERSION(_x,_p,_v) \
(_x).s.pointer = _p; (_x).s.version = _v


#elif defined(__x86_64__) || defined(__ia64__)

#define ALPOINTER(_x) ((void*)(((((intptr_t)(_x).data)<<16)>>16) | \
 (((~((((intptr_t)(_x).data)<<16>>63)-1))>>48)<<48)))
#define ALVERSION(_x) (((intptr_t)(_x).data)>>48)
#define SET_ATOMICLIST_POINTER_VERSION(_x,_p,_v) \
  (_x).data = ((((intptr_t)(_p))&0x0000FFFFFFFFFFFFULL) | (((_v)&0xFFFFULL) << 48))
  
#else

#error "unsupported processor"

#endif

void atomic_list_init(atomiclist *al, char *name, int next_offset);
void *atomic_list_push(atomiclist *al, void *data);
void *atomic_list_pop(atomiclist *al);
void *atomic_list_popall(atomiclist *al);
bool atomic_list_empty(atomiclist *al);

#endif
