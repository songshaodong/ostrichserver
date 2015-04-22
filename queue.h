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

#ifndef _QUEUE_H_
#define _QUEUE_H_

#define HAS_128BIT_CAS 1   // todo test 128bit cas support.

typedef union
  {
#if (defined(__i386__) || defined(__arm__)) && (SIZEOF_VOIDP == 4)
    struct
    {
      void *pointer;
      int32_t version;
    } s;
    int64_t data;
#elif HAS_128BIT_CAS
    struct
    {
      void *pointer;
      int64_t version;
    } s;
    __int128_t data;
#else
    int64_t data;
#endif
  } head_p;
  
typedef struct
{
    volatile head_p head;
    const char     *name;
    uint32_t        offset;
} atomiclist;

#endif