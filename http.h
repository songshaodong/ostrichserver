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

typedef struct {
    char                  *name;
    uint32_t               offset;
    int                  (*header_handler)(uint32_t offset);
} http_header_t;

typedef struct {
} header_p;

/*typedef struct {
    ngx_list_t                 headers;

    header_p                  *host;
    header_p                  *connection;
    header_p                  *if_modified_since;
    header_p                  *if_unmodified_since;
    header_p                  *if_match;
    header_p                  *if_none_match;
    header_p                  *user_agent;
    header_p                  *referer;
    header_p                  *content_length;
    header_p                  *content_type;

    header_p                  *range;
    header_p                  *if_range;

    header_p                  *transfer_encoding;
    header_p                  *expect;
    header_p                  *upgrade;

    header_p                  *authorization;

    header_p                  *keep_alive;


    header_p                  *accept;
    header_p                  *accept_language;

    char                      *server;
    off_t                      content_length_n;
    time_t                     keep_alive_n;
} http_request_headers;

typedef struct {
} http_response_headers;
*/
void http_init();

#endif
