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

void spin_lock(atomic_t *lock, atomic_int value)
{

    uint32_t  i, n;

    for ( ;; ) {

        if (*lock == 0 && atomic_cas(lock, 0, value)) {
            return;
        }

        if (cpu_num > 1) {

            for (n = 1; n < pid; n <<= 1) {

                for (i = 0; i < n; i++) {
                    cpu_pause();
                }

                if (*lock == 0 && atomic_cas(lock, 0, value)) {
                    return;
                }
            }
        }

        /*   causes the calling thread to relinquish the CPU.  
		  *  The thread is moved to the end of the queue for its static priority and a new thread gets to run.
		  */
       sched_yield();
    }
}

