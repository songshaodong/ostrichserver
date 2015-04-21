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

#include <common.h>
#include <thread.h>

int thread_create(evthread *evt, int stacksize, int detached, int type)
{
    int ret;
    
    pthread_attr_t attr;

    pthread_attr_init(&attr);
  
    pthread_attr_setstacksize(&attr, stacksize);

    if (detached) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }
    
    ret = pthread_create(&evt->tid, &attr, evt->execute, evt);
    if (ret < 0) {
        return OS_ERR;
    }

    pthread_attr_destroy(&attr);

    return OS_OK;
}

evthread *make_thread_pool(threadproc exec, int evtype, int num)
{
    evthread *evt;
    evthread *t;
    int       i;
    int       ret;
    
    evt = os_malloc(num * sizeof(evthread));
    if (evt == NULL) {
        return NULL;
    }

    for (i = 0; i < num; i++) {
        t = evt + i;
        t->execute = exec;
        ret = thread_create(evt, STACK_SIZE, 1, evtype);
        if (ret == OS_ERR) {
            return NULL;
        }
    }
    
    return evt;
}
