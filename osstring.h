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

#ifndef _STRING_H_
#define _STRING_H_

#include "common.h"

#define null_string   { 0, NULL }

typedef struct {
    char *data;
    int   size;
} string;

typedef struct {
    string   name;
    string   value;
} stringkv;

char *os_strdup(char *str);
char *os_strndup(char *str, size_t n);
ssize_t os_natoi(char *line, size_t n);
ssize_t os_atoi(char *line);

#endif