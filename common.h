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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <assert.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <stddef.h>
#include <signal.h>
#include <sys/eventfd.h>
#include <stdint.h>
#include <limits.h>

#ifndef likely
#define likely(x)	__builtin_expect (!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)	__builtin_expect (!!(x), 0)
#endif

#define COL(x,y)  "\033[" #x ";" #y "m"
#define RED     COL(31,1)
#define GREEN   COL(32,1)
#define YELLOW  COL(0,33)
#define WHITE   COL(0,1)
#define GRAY    "\033[0m"

enum OS_ERRNO {
    OS_OK = 0,
    OS_ERR = -1,
	OS_AGAIN = -2,
	OS_BUSY = -3,
	OS_DONE = -4,
	OS_DECLINED = -5,
	OS_ABORT = -6
};

typedef int os_socket;

typedef uint32_t msec;

#define LF     (u_char) 10
#define CR     (u_char) 13
#define CRLF   "\x0d\x0a"

#define true 1
#define false 0

#define bool int
#define os_uint_t uint32_t

// typedef struct lists
typedef struct thread evthread;
typedef struct external_queue   externalq;
typedef struct localqueue  localq;
typedef struct thread_event   event;
typedef struct evtype  evtype;
typedef struct thread_processor processor;
typedef struct thread_dedicated  dedthread;
typedef struct continuation continuation;
typedef struct connection  connection;
typedef struct event_list  evlink;
typedef struct buffer os_buf;


// typedef function lists
typedef void *(*threadproc)(void *);
typedef void (*type_handler)();
typedef void (*conthandler)(event *);


#include "hashtable.h"
#include "memory.h"
#include "queue.h"
#include "event.h"
#include "continuation.h"
#include "thread.h"
#include "osstring.h"
#include "eventpoll.h"
#include "net.h"
#include "mutex.h"
#include "atomiclist.h"
#include "atomic.h"
#include "processor.h"
#include "osbuf.h"
#include "oserrno.h"
#include "arrays.h"
#include "dict.h"


extern _eventprocessor evprocessor;
//extern hashtable *config_record_hashtable;
extern dict *config_hashtable;
extern thread_key_t thread_private_key;
extern __thread priority_queue event_priority_queue;
extern int workerid;
extern int workerstatus;
extern int masterid;
extern int quiet;
extern int reconfig;
extern int restart;
extern int cf_daemon;

int os_daemon();

#endif
