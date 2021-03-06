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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "common.h"

typedef union {
    int     config_int;
    float   config_float;
    char   *config_string;
} config_data;

enum {
    RECORD_INT = 1,
    RECORD_STRING,
    RECORD_FLOAT
};

typedef struct {
    config_data data;
    int         type;
    string      name;
} record;

inline record *get_config_record(string *str);
unsigned int config_hash_key(void *key);

//int config_hashtable_init();

int config_parse_file();
    
#endif
