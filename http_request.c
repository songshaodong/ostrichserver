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

#define MAX_REQUEST_LINE_SIZE 1024


static int
http_process_request_uri(http_request *r);

static int
os_http_validate_host(string *host, os_uint_t alloc);

void
http_process_request(http_request *r);

static void
http_process_request_line(event *rev);

static void
http_process_request_headers(event *ev);

static void*
http_create_request(connection *c)
{
	http_request *r;
	
	r = (http_request*)os_calloc(sizeof(http_request));

	r->count++;
	r->header_in = (os_buf *)&c->buf;
	
	return r;
}

static void
http_free_request(http_request* r){

	/*TODO: */
	
	os_free(r);
}

static void
http_close_connection(connection* c){


	if (c->fd == (os_socket)-1) {
		return;
	}

	printf(GREEN "[%lu] close http connection: %d \n" GRAY, 
			pthread_self(), c->fd);
	
	close(c->fd);
	
	if (c->buf->start) {
		os_free(c->buf->start);
	}
    
	os_free(c);
}

void
http_close_request(http_request *r, int rc)
{
    connection  *c;

	c = get_connection(r);


	printf(GREEN "[%lu] http request count:%d \n" GRAY, 
			pthread_self(), r->count);

    if (r->count == 0) {

		printf(RED "[%lu] http request count is zero \n" GRAY, pthread_self());
    }

    r->count--;

    if (r->count) {
        return;
    }

    http_free_request(r);
    http_close_connection(c);
}

void
http_finalize_request(http_request *r, int rc)
{
	printf(GREEN "[%lu] http finalize request \n" GRAY, pthread_self());
}

void
http_wait_request_handler(event *ev)
{
	http_request	*r;
	os_buf			*b;
	int				 n;
	connection 		*c;
	netconnection	*nc;

	c = (connection *)ev->cont;
	nc = (netconnection *)ev->cont;
		
	b = (os_buf *)&c->buf;
	
	b->start = (u_char*)os_calloc(MAX_REQUEST_LINE_SIZE);
	if (b->start == NULL) {
	    http_close_connection(c);
	    return;
	}

	b->pos = b->start;
	b->last = b->start;
	b->end = b->last + MAX_REQUEST_LINE_SIZE;

	n = os_recv(c, b->start, MAX_REQUEST_LINE_SIZE);
	if (n == OS_AGAIN) {
			
	   if (!ev->timer_set) {
		   add_timer(ev, ev->timeout);
	   }

	   if (epoll_event_add(ev, 0) != OS_OK) {
		   http_close_connection(c);
		   return;
	   }
	   
	   return;
	}

	if (n == OS_ERR) {

	   printf(RED "[%lu] recv error: %s \n" GRAY, 
			   pthread_self(), strerror(os_socket_errno));

	   http_close_connection(c);
	   return;
	}

	if (n == 0) {
	   http_close_connection(c);
	   return;
	}

	b->last += n;
	
	r = http_create_request(c);
	if (r == NULL) {
		http_close_connection(c);
		return;
	}

	r->cont.private_data = ev->cont;

	ev->cont = (continuation*)r;
	ev->cont->event_handler = http_process_request_line;
	
	http_process_request_line(ev);	
}


static ssize_t
http_read_request_header(http_request *r)
{
    int                      n;
    event                   *rev;
    connection              *c;
	msec	                 client_header_timeout;

    printf(GREEN "[%lu] read request \n" GRAY, pthread_self());

    c = get_connection(r);
	rev = c->read;

    n = r->header_in->last - r->header_in->pos;

    if (n > 0) {
        return n;
    }

    n = os_recv(c, r->header_in->last,
                r->header_in->end - r->header_in->last);

    if (n == OS_AGAIN) {
        if (!rev->timer_set) {
			/* TODO: */
			client_header_timeout = 5000;
            add_timer(rev, client_header_timeout);
        }

        if (epoll_event_add(rev, 0) != OS_OK) {
            http_close_request(r, OS_HTTP_INTERNAL_SERVER_ERROR);
            return OS_ERR;
        }

        return OS_AGAIN;
    }

    if (n == 0) {

		printf(RED "[%lu] client prematurely closed connection. \n" GRAY, pthread_self());
    }

    if (n == 0 || n == OS_ERR) {
        c->error = 1;
        http_finalize_request(r, OS_HTTP_BAD_REQUEST);
        return OS_ERR;
    }

    r->header_in->last += n;

    return n;
}

static void
http_process_request_line(event *rev)
{
	ssize_t           n;
    int               rc, rv;
    string            host;
	connection		 *c;
	http_request	 *r;

	r = (http_request *)rev->cont;
	c = get_connection(r);

   	printf(GREEN "[%lu] http process request line \n" GRAY, pthread_self());
   
    if (rev->timedout) {
   		printf(GREEN "[%lu] client timed out \n" GRAY, pthread_self());

        c->timedout = 1;
        http_close_request(r, OS_HTTP_REQUEST_TIME_OUT);
        return;
    }

    rc = OS_AGAIN;

    for ( ;; ) {

        if (rc == OS_AGAIN) {
			
            n = http_read_request_header(r);
			
            if (n == OS_AGAIN || n == OS_ERR) {
                return;
            }
        }

        rc = http_parse_request_line(r, r->header_in);

        if (rc == OS_OK) {

            r->request_line.len = r->request_end - r->request_start;
            r->request_line.data = r->request_start;
            r->request_length = r->header_in->pos - r->request_start;
            r->method_name.len = r->method_end - r->request_start + 1;
            r->method_name.data = r->request_line.data;

            if (r->http_protocol.data) {
                r->http_protocol.len = r->request_end - r->http_protocol.data;
            }

            if (http_process_request_uri(r) != OS_OK) {
                return;
            }

            if (r->host_start && r->host_end) {

                host.len = r->host_end - r->host_start;
                host.data = r->host_start;

                rc = os_http_validate_host(&host, 0);

                if (rc == OS_DECLINED) {
                    http_finalize_request(r, OS_HTTP_BAD_REQUEST);
                    return;
                }

                if (rc == OS_ERR) {
                    http_close_request(r, OS_HTTP_INTERNAL_SERVER_ERROR);
                    return;
                }

            }

            if (r->http_version < OS_HTTP_VERSION_10) {
                http_process_request(r);
                return;
            }
			
            rev->cont->event_handler = http_process_request_headers;
            http_process_request_headers(rev);

            return;
        }

        if (rc != OS_AGAIN) {
            http_finalize_request(r, OS_HTTP_BAD_REQUEST);
            return;
        }  
    }
}

static void
http_process_request_headers(event *ev)
{
	printf(GREEN "[%lu] http process request headers \n" GRAY, pthread_self());
}


void
http_process_request(http_request *r)
{
	printf(GREEN "[%lu] http process request \n" GRAY, pthread_self());
}


int
http_process_request_uri(http_request *r)
{
    if (r->args_start) {
        r->uri.len = r->args_start - 1 - r->uri_start;
    } else {
        r->uri.len = r->uri_end - r->uri_start;
    }

    if (r->complex_uri || r->quoted_uri) {
		
        r->uri.data = os_malloc(r->uri.len + 1);
        if (r->uri.data == NULL) {
            http_close_request(r, OS_HTTP_INTERNAL_SERVER_ERROR);
            return OS_ERR;
        }

		/* TODO: require merge uri of slashes ? */ 

    } else {
        r->uri.data = r->uri_start;
    }

    r->unparsed_uri.len = r->uri_end - r->uri_start;
    r->unparsed_uri.data = r->uri_start;

    r->valid_unparsed_uri = r->space_in_uri ? 0 : 1;

    if (r->uri_ext) {
        if (r->args_start) {
            r->exten.len = r->args_start - 1 - r->uri_ext;
        } else {
            r->exten.len = r->uri_end - r->uri_ext;
        }

        r->exten.data = r->uri_ext;
    }

    if (r->args_start && r->uri_end > r->args_start) {
        r->args.len = r->uri_end - r->args_start;
        r->args.data = r->args_start;
    }


	/*
	
    os_log_debug1(OS_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http uri: \"%V\"", &r->uri);

    os_log_debug1(OS_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http args: \"%V\"", &r->args);

    os_log_debug1(OS_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http exten: \"%V\"", &r->exten);
    */

    return OS_OK;
}

static int
os_http_validate_host(string *host, os_uint_t alloc)
{
    u_char  *h, ch;
    size_t   i, dot_pos, host_len;

    enum {
        sw_usual = 0,
        sw_literal,
        sw_rest
    } state;

    dot_pos = host->len;
    host_len = host->len;

    h = host->data;

    state = sw_usual;

    for (i = 0; i < host->len; i++) {
        ch = h[i];

        switch (ch) {

        case '.':
            if (dot_pos == i - 1) {
                return OS_DECLINED;
            }
            dot_pos = i;
            break;

        case ':':
            if (state == sw_usual) {
                host_len = i;
                state = sw_rest;
            }
            break;

        case '[':
            if (i == 0) {
                state = sw_literal;
            }
            break;

        case ']':
            if (state == sw_literal) {
                host_len = i + 1;
                state = sw_rest;
            }
            break;

        case '\0':
            return OS_DECLINED;

        default:

            if (os_path_separator(ch)) {
                return OS_DECLINED;
            }

            if (ch >= 'A' && ch <= 'Z') {
                alloc = 1;
            }

            break;
        }
    }

    if (dot_pos == host_len - 1) {
        host_len--;
    }

    if (host_len == 0) {
        return OS_DECLINED;
    }

    if (alloc) {
        host->data = os_malloc(host_len);
        if (host->data == NULL) {
            return OS_ERR;
        }

        os_strlow(host->data, h, host_len);
    }

    host->len = host_len;

    return OS_OK;
}
