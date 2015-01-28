/* 
   Copyright (c) 2014-2015 flashbuckets 

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

#include <common.h>
#include <acceptor.h>

void *accept_main(void *data) 
{
   do {
       sleep(1);
   } while (1);
}

int accept_thread_init(int n)
{
    int i;

    evthread *evt;

    for (i = 0; i < n; i++) {
        evt = os_calloc(sizeof(evthread));
        evt->handler = accept_main;
        thread_create(evt, STACK_SIZE, 0, DEDICATED);
    }

    return OS_OK;
}