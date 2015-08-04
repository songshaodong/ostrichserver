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

#ifndef _ERRNO_H_
#define _ERRNO_H_


#define OS_EPERM         EPERM
#define OS_ENOENT        ENOENT
#define OS_ENOPATH       ENOENT
#define OS_ESRCH         ESRCH
#define OS_EINTR         EINTR
#define OS_ECHILD        ECHILD
#define OS_ENOMEM        ENOMEM
#define OS_EACCES        EACCES
#define OS_EBUSY         EBUSY
#define OS_EEXIST        EEXIST
#define OS_EXDEV         EXDEV
#define OS_ENOTDIR       ENOTDIR
#define OS_EISDIR        EISDIR
#define OS_EINVAL        EINVAL
#define OS_ENFILE        ENFILE
#define OS_EMFILE        EMFILE
#define OS_ENOSPC        ENOSPC
#define OS_EPIPE         EPIPE
#define OS_EINPROGRESS   EINPROGRESS
#define OS_ENOPROTOOPT   ENOPROTOOPT
#define OS_EOPNOTSUPP    EOPNOTSUPP
#define OS_EADDRINUSE    EADDRINUSE
#define OS_ECONNABORTED  ECONNABORTED
#define OS_ECONNRESET    ECONNRESET
#define OS_ENOTCONN      ENOTCONN
#define OS_ETIMEDOUT     ETIMEDOUT
#define OS_ECONNREFUSED  ECONNREFUSED
#define OS_ENAMETOOLONG  ENAMETOOLONG
#define OS_ENETDOWN      ENETDOWN
#define OS_ENETUNREACH   ENETUNREACH
#define OS_EHOSTDOWN     EHOSTDOWN
#define OS_EHOSTUNREACH  EHOSTUNREACH
#define OS_ENOSYS        ENOSYS
#define OS_ECANCELED     ECANCELED
#define OS_EILSEQ        EILSEQ
#define OS_ENOMOREFILES  0
#define OS_ELOOP         ELOOP
#define OS_EBADF         EBADF

#if (OS_HAVE_OPENAT)
#define OS_EMLINK        EMLINK
#endif

#if (__hpux__)
#define OS_EAGAIN        EWOULDBLOCK
#else
#define OS_EAGAIN        EAGAIN
#endif


#define os_errno                  errno
#define os_socket_errno           errno
#define os_set_errno(err)         errno = err
#define os_set_socket_errno(err)  errno = err

#endif 
