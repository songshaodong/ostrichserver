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

#ifndef _HTTP_H_
#define _HTTP_H_

#define   MAX_HEADERS  512

typedef struct {

} http_request_headers_out;

typedef struct header_callback headercb;

struct header_callback {
     int           (*handler)(uint32_t offset);
     headercb       *next;
}; 

typedef struct {
    int         hash;
    int         metaidx;
    stringkv    headerkv;
    headercb    callback;
} http_header;

typedef struct {
    http_header          *host;
} http_request_headers_in;

typedef struct {
    string                    uri;
    http_request_headers_in   headersin;
    http_request_headers_out  headersout;
} http_request;

typedef struct {
    char                *name;
    uint32_t             offset;
    int                (*header_parser)(http_request *r, http_header *h, uint32_t offset);
} http_header_meta;

void http_init();
int http_process_unique_header_line(http_request *r, http_header *h, uint32_t offset);

#endif
