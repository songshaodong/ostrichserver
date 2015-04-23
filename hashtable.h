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

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "common.h"

#define MAXHASHTABLELEN 1024

typedef struct {
    int32_t    nhashA;
    int32_t    nhashB;
    uint8_t    exist;
    void      *data;
} hashitem;

typedef struct {
    hashitem  *buckets;
    uint32_t   size;
} hashtable;

int hashtable_isexist(char *string, hashtable *htable);
int hashtable_add(char *string, void *data, hashtable *htable);
int hashtable_init(hashitem **pphashtable, int ntablelength);
    
#endif