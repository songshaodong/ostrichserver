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
#include "timer.h"

// return microseconds
hrtime_t get_current_time()
{
    //struct timeval tv;
    
    //gettimeofday(&tv, NULL);
    
    //return (tv.tv_sec * 1000 * 1000  + tv.tv_usec);

    hrtime_t tm;

    clock_gettime(CLOCK_REALTIME, &tm);

    return tm;
}

