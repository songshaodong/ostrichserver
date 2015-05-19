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
#include "http.h"

http_header_meta  http_headers[] = {
    { "Host", offsetof(http_request_headers_in, host),
                 http_process_unique_header_line },

    { NULL, -1, NULL }
};

hashtable *http_header_hashtable;

void http_header_init()
{
}

int http_header_hashtable_init(int size)
{
    int result;
    http_header_meta *header;

    (void) size;

    http_header_hashtable = os_calloc(sizeof(hashtable));
    http_header_hashtable->size = MAX_HEADERS;
    
    result = hashtable_init(&http_header_hashtable->buckets, 
        http_header_hashtable->size);
    
    if (!result) {
        return OS_ERR;
    }

    for (header = http_headers; header->name != NULL; header++) { 
        hashtable_add(header->name, header, http_header_hashtable);
    }
    
    return OS_OK;
}

void http_init()
{
    http_header_hashtable_init(1);
    http_header_init();
}

int http_process_unique_header_line(http_request *r, http_header *h, 
    uint32_t offset)
{
    http_header **header;
    
    header = (http_header **)((char *) &r->headersin + offset);

    *header = h;

    return OS_OK;
}
