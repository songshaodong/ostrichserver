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

#include <common.h>
#include <config.h>
#include <hashtable.h>

extern hashtable config_record_hashtable;

int main()
{
    int         rc = 0;
    hashitem   *hi;
    
    rc = config_parse_file("a.txt");

    rc = hashtable_isexist("net.tcp.connection", &config_record_hashtable);

    if (rc != -1) {
        hi = config_record_hashtable.buckets + rc;
        printf("name: %s\n", ((record *)hi->data)->name);
    }
    
    return 0;
}