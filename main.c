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
#include "config.h"

extern hashtable *config_record_hashtable;

extern thread_key_t thread_private_key;

int main()
{
    int          result = 0;
    record      *rec;
    int          listenfd = 0;

    pthread_key_create(&thread_private_key, NULL);
    
    result = config_parse_file("record.config");

    eventprocessor_init(DEFAULT_THREADS);

    acceptor_init();
    
    while (1) {
        sleep(1);
    }
    
    return 0;
}
