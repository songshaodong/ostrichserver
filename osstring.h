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

#ifndef _STRING_H_
#define _STRING_H_

#include "common.h"

#define null_string   { 0, NULL }
#define os_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define os_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)


void os_strlow(u_char *dst, u_char *src, size_t n);


#define os_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define os_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)


#define os_strstr(s1, s2)  strstr((const char *) s1, (const char *) s2)
#define os_strlen(s)       strlen((const char *) s)

#define os_strchr(s1, c)   strchr((const char *) s1, (int) c)

static inline u_char *
os_strlchr(u_char *p, u_char *last, u_char c)
{
    while (p < last) {

        if (*p == c) {
            return p;
        }

        p++;
    }

    return NULL;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#define os_memzero(buf, n)       (void) memset(buf, 0, n)


#if (OS_MEMCPY_LIMIT)

void *os_memcpy(void *dst, const void *src, size_t n);
#define os_cpymem(dst, src, n)   (((u_char *) os_memcpy(dst, src, n)) + (n))

#else

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define os_memcpy(dst, src, n)   (void) memcpy(dst, src, n)
#define os_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#endif


#if ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

static inline u_char *
os_copy(u_char *dst, u_char *src, size_t len)
{
    if (len < 17) {

        while (len) {
            *dst++ = *src++;
            len--;
        }

        return dst;

    } else {
        return os_cpymem(dst, src, len);
    }
}

#else

#define os_copy                  os_cpymem

#endif


#define os_memmove(dst, src, n)   (void) memmove(dst, src, n)
#define os_movemem(dst, src, n)   (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define os_memcmp(s1, s2, n)  memcmp((const char *) s1, (const char *) s2, n)


typedef struct {
    u_char *data;
    int     len;
} string;

typedef struct {
    string   name;
    string   value;
} stringkv;

char *os_strdup(char *str);
char *os_strndup(char *str, size_t n);
ssize_t os_natoi(char *line, size_t n);
ssize_t os_atoi(char *line);


void os_strlow(u_char *dst, u_char *src, size_t n);
os_uint_t os_strncasecmp(u_char *s1, u_char *s2, size_t n);
u_char *os_strstrn(u_char *s1, char *s2, size_t n);
u_char *os_strcasestrn(u_char *s1, char *s2, size_t n);

#if (OS_MEMCPY_LIMIT)
void *os_memcpy(void *dst, const void *src, size_t n);
#endif

#endif
