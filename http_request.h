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

#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_


#define os_path_separator(c)    ((c) == '/')

/* must be 2^n */
#define OS_HTTP_LC_HEADER_LEN             32

#define OS_HTTP_VERSION_9                 9
#define OS_HTTP_VERSION_10                1000
#define OS_HTTP_VERSION_11                1001

#define OS_HTTP_UNKNOWN                   0x0001
#define OS_HTTP_GET                       0x0002
#define OS_HTTP_HEAD                      0x0004
#define OS_HTTP_POST                      0x0008
#define OS_HTTP_PUT                       0x0010
#define OS_HTTP_DELETE                    0x0020
#define OS_HTTP_MKCOL                     0x0040
#define OS_HTTP_COPY                      0x0080
#define OS_HTTP_MOVE                      0x0100
#define OS_HTTP_OPTIONS                   0x0200
#define OS_HTTP_PROPFIND                  0x0400
#define OS_HTTP_PROPPATCH                 0x0800
#define OS_HTTP_LOCK                      0x1000
#define OS_HTTP_UNLOCK                    0x2000
#define OS_HTTP_PATCH                     0x4000
#define OS_HTTP_TRACE                     0x8000

#define OS_HTTP_BAD_REQUEST               400
#define OS_HTTP_UNAUTHORIZED              401
#define OS_HTTP_FORBIDDEN                 403
#define OS_HTTP_NOT_FOUND                 404
#define OS_HTTP_NOT_ALLOWED               405
#define OS_HTTP_REQUEST_TIME_OUT          408
#define OS_HTTP_CONFLICT                  409
#define OS_HTTP_LENGTH_REQUIRED           411
#define OS_HTTP_PRECONDITION_FAILED       412
#define OS_HTTP_REQUEST_ENTITY_TOO_LARGE  413
#define OS_HTTP_REQUEST_URI_TOO_LARGE     414
#define OS_HTTP_UNSUPPORTED_MEDIA_TYPE    415
#define OS_HTTP_RANGE_NOT_SATISFIABLE     416


#define OS_HTTP_INTERNAL_SERVER_ERROR     500
#define OS_HTTP_NOT_IMPLEMENTED           501
#define OS_HTTP_BAD_GATEWAY               502
#define OS_HTTP_SERVICE_UNAVAILABLE       503
#define OS_HTTP_GATEWAY_TIME_OUT          504
#define OS_HTTP_INSUFFICIENT_STORAGE      507

#define OS_HTTP_CONNECTION_CLOSE          1
#define OS_HTTP_CONNECTION_KEEP_ALIVE     2

#define OS_HTTP_PARSE_HEADER_DONE         1

#define OS_HTTP_CLIENT_ERROR              10
#define OS_HTTP_PARSE_INVALID_METHOD      10
#define OS_HTTP_PARSE_INVALID_REQUEST     11
#define OS_HTTP_PARSE_INVALID_09_METHOD   12

#define OS_HTTP_PARSE_INVALID_HEADER      13

void
http_wait_request_handler(event *ev);

#endif /* _HTTP_REQUEST_H_ */
