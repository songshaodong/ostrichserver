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
		http_process_unique_header_line, NULL },

	{ "Connection", offsetof(http_request_headers_in, connection),
		http_process_connection, NULL },

	{ "If-Modified-Since",
		offsetof(http_request_headers_in, if_modified_since),
		http_process_unique_header_line, NULL },

	{ "If-Unmodified-Since",
		offsetof(http_request_headers_in, if_unmodified_since),
		http_process_unique_header_line, NULL },

	{ "If-Match",
		offsetof(http_request_headers_in, if_match),
		http_process_unique_header_line, NULL },

	{ "If-None-Match",
		offsetof(http_request_headers_in, if_none_match),
		http_process_unique_header_line, NULL },

	{ "User-Agent", offsetof(http_request_headers_in, user_agent),
		http_process_user_agent, NULL },

	{ "Referer", offsetof(http_request_headers_in, referer),
		http_process_header_line, NULL },

	{ "Content-Length",
		offsetof(http_request_headers_in, content_length),
		http_process_unique_header_line, NULL },

	{ "Content-Type",
		offsetof(http_request_headers_in, content_type),
		http_process_header_line, NULL },

	{ "Range", offsetof(http_request_headers_in, range),
		http_process_header_line, NULL },

	{ "If-Range",
		offsetof(http_request_headers_in, if_range),
		http_process_unique_header_line, NULL },

	{ "Transfer-Encoding",
		offsetof(http_request_headers_in, transfer_encoding),
		http_process_header_line, NULL },

	{ "Expect",
		offsetof(http_request_headers_in, expect),
		http_process_unique_header_line, NULL },

	{ "Upgrade",
		offsetof(http_request_headers_in, upgrade),
		http_process_header_line, NULL },

#if (HTTP_GZIP)
	{ "Accept-Encoding",
		offsetof(http_request_headers_in, accept_encoding),
		http_process_header_line, NULL },

	{ "Via", offsetof(http_request_headers_in, via),
		http_process_header_line, NULL },
#endif

	{ "Authorization",
		offsetof(http_request_headers_in, authorization),
		http_process_unique_header_line, NULL },

	{ "Keep-Alive", offsetof(http_request_headers_in, keep_alive),
		http_process_header_line, NULL },

#if (HTTP_X_FORWARDED_FOR)
	{ "X-Forwarded-For",
		offsetof(http_request_headers_in, x_forwarded_for),
		http_process_multi_header_lines, NULL },
#endif

#if (HTTP_REALIP)
	{ "X-Real-IP",
		offsetof(http_request_headers_in, x_real_ip),
		http_process_header_line, NULL },
#endif

#if (HTTP_HEADERS)
	{ "Accept", offsetof(http_request_headers_in, accept),
		http_process_header_line, NULL },

	{ "Accept-Language",
		offsetof(http_request_headers_in, accept_language),
		http_process_header_line, NULL },
#endif

#if (HTTP_DAV)
	{ "Depth", offsetof(http_request_headers_in, depth),
		http_process_header_line },

	{ "Destination", offsetof(http_request_headers_in, destination),
		http_process_header_line, NULL },

	{ "Overwrite", offsetof(http_request_headers_in, overwrite),
		http_process_header_line, NULL },

	{ "Date", offsetof(http_request_headers_in, date),
		http_process_header_line, NULL },
#endif

	{ "Cookie", offsetof(http_request_headers_in, cookies),
		http_process_multi_header_lines, NULL },

	{ NULL, -1, NULL, NULL }
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

static int
http_process_unique_header_line(http_request *r, http_header *h, 
    uint32_t offset)
{
    http_header **header;
    
    header = (http_header **)((char *) &r->headers_in + offset);

    *header = h;

    return OS_OK;
}

static int
http_process_connection(http_request *r, http_header *h,
	uint32_t offset)
{
	if (os_strcasestrn(h->headerkv.value.data, "close", 5 - 1)) {
		r->headers_in.connection_type = OS_HTTP_CONNECTION_CLOSE;

	} else if (os_strcasestrn(h->headerkv.value.data, "keep-alive", 10 - 1)) {
		r->headers_in.connection_type = OS_HTTP_CONNECTION_KEEP_ALIVE;
	}

	return OS_OK;
}

static int
http_process_user_agent(http_request *r, http_header *h,
	uint32_t offset)
{
	u_char  *user_agent, *msie;

	if (r->headers_in.user_agent) {
		return OS_OK;
	}

	r->headers_in.user_agent = h;

	/* check some widespread browsers while the header is in CPU cache */

	user_agent = h->headerkv.value.data;

	msie = os_strstrn(user_agent, "MSIE ", 5 - 1);

	if (msie && msie + 7 < user_agent + h->headerkv.value.len) {

		r->headers_in.msie = 1;

		if (msie[6] == '.') {

			switch (msie[5]) {
				case '4':
				case '5':
					r->headers_in.msie6 = 1;
					break;
				case '6':
					if (os_strstrn(msie + 8, "SV1", 3 - 1) == NULL) {
						r->headers_in.msie6 = 1;
					}
					break;
			}
		}

	}

	if (os_strstrn(user_agent, "Opera", 5 - 1)) {
		r->headers_in.opera = 1;
		r->headers_in.msie = 0;
		r->headers_in.msie6 = 0;
	}

	if (!r->headers_in.msie && !r->headers_in.opera) {

		if (os_strstrn(user_agent, "Gecko/", 6 - 1)) {
			r->headers_in.gecko = 1;

		} else if (os_strstrn(user_agent, "Chrome/", 7 - 1)) {
			r->headers_in.chrome = 1;

		} else if (os_strstrn(user_agent, "Safari/", 7 - 1)
				&& os_strstrn(user_agent, "Mac OS X", 8 - 1))
		{
			r->headers_in.safari = 1;

		} else if (os_strstrn(user_agent, "Konqueror", 9 - 1)) {
			r->headers_in.konqueror = 1;
		}
	}

	return OS_OK;
}

static int
http_process_header_line(http_request *r, http_header *h,
	uint32_t offset)
{
	http_header  **ph;

	ph = (http_header **) ((char *) &r->headers_in + offset);

	if (*ph == NULL) {
		*ph = h;
	}

	return OS_OK;
}

static int
http_process_multi_header_lines(http_request *r, http_header *h,
	uint32_t offset)
{
	http_header  		**ph;
	http_header_array    *headers;
	
	headers = (http_header_array *) ((char *) &r->headers_in + offset);

	if (headers->elts == NULL) {
		TAB_INIT(headers->nelts, headers->elts);
	}

	TAB_APPEND(headers->nelts, headers->elts, h);

	return OS_OK;
}
