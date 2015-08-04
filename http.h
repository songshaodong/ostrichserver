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

#include "http_request.h"

#define   MAX_HEADERS  512

#define  get_connection(r) ((connection *)r->cont.private_data)

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
} http_header;

typedef struct {
	int			   nelts;
	http_header  **elts;
} http_header_array;

typedef struct {
    http_header          		 *host;
	http_header                  *connection;
	http_header                  *if_modified_since;
	http_header                  *if_unmodified_since;
	http_header                  *if_match;
	http_header                  *if_none_match;
	http_header                  *user_agent;
	http_header                  *referer;
	http_header                  *content_length;
	http_header                  *content_type;

	http_header                  *range;
	http_header                  *if_range;

	http_header                  *transfer_encoding;
	http_header                  *expect;
	http_header                  *upgrade;

#if (HTTP_GZIP)
	http_header                  *accept_encoding;
	http_header                  *via;
#endif

	http_header                  *authorization;

	http_header                  *keep_alive;

#if (HTTP_X_FORWARDED_FOR)
	http_header_array             x_forwarded_for;
#endif

#if (HTTP_REALIP)
	http_header                  *x_real_ip;
#endif

#if (HTTP_HEADERS)
	http_header                  *accept;
	http_header                  *accept_language;
#endif

#if (HTTP_DAV)
	http_header                  *depth;
	http_header                  *destination;
	http_header                  *overwrite;
	http_header                  *date;
#endif

	http_header_array             cookies;
	
	unsigned                          connection_type:2;
	unsigned                          chunked:1;
	unsigned                          msie:1;
	unsigned                          msie6:1;
	unsigned                          opera:1;
	unsigned                          gecko:1;
	unsigned                          chrome:1;
	unsigned                          safari:1;
	unsigned                          konqueror:1;

} http_request_headers_in;

typedef struct {
	continuation			         cont;

	os_buf                          *header_in;
	uint32_t                	     method;
	uint32_t                         http_version;

	off_t                            request_length;
	string                           request_line;
	string                           uri;
	string                           args;
	string                           exten;
	string                           unparsed_uri;

	string                           method_name;
	string                           http_protocol;

	uint32_t					     count;

	/*unsigned                          blocked:8; */


	unsigned                          http_state:4;

	unsigned                          invalid_header:1;

	unsigned                          add_uri_to_alias:1;
	unsigned                          valid_location:1;
	unsigned                          valid_unparsed_uri:1;


	/* URI with "/." and on Win32 with "//" */
	unsigned                          complex_uri:1;

	/* URI with "%" */
	unsigned                          quoted_uri:1;

	/* URI with "+" */
	unsigned                          plus_in_uri:1;

	/* URI with " " */
	unsigned                          space_in_uri:1;

	/* used to parse HTTP headers */

	uint32_t                          state;

	uint32_t                          header_hash;
	uint32_t                          lowcase_index;
	u_char                            lowcase_header[OS_HTTP_LC_HEADER_LEN];

	u_char                           *header_name_start;
	u_char                           *header_name_end;
	u_char                           *header_start;
	u_char                           *header_end;

	/*
	 * a memory that can be reused after parsing a request line
	 * via os_http_ephemeral_t
	 */

	u_char                           *uri_start;
	u_char                           *uri_end;
	u_char                           *uri_ext;
	u_char                           *args_start;
	u_char                           *request_start;
	u_char                           *request_end;
	u_char                           *method_end;
	u_char                           *schema_start;
	u_char                           *schema_end;
	u_char                           *host_start;
	u_char                           *host_end;
	u_char                           *port_start;
	u_char                           *port_end;

	unsigned                          http_minor:16;
	unsigned                          http_major:16;

	http_request_headers_in           headers_in;
	http_request_headers_out          headers_out;

} http_request;

typedef struct {
    char                *name;
    uint32_t             offset;
    int                (*header_parser)(http_request *r, http_header *h, uint32_t offset);
    headercb            *callback;
} http_header_meta;

void http_init();

static 
int http_process_unique_header_line(http_request *r, http_header *h, uint32_t offset);

static int
http_process_multi_header_lines(http_request *r, http_header *h, uint32_t offset);

static int
http_process_connection(http_request *r, http_header *h, uint32_t offset);

static int
http_process_user_agent(http_request *r, http_header *h, uint32_t offset);

static int
http_process_header_line(http_request *r, http_header *h, uint32_t offset);

#endif
