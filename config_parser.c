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
#include <config_parser.h>
#include <memory.h>

static int 
config_loadfile(char *filepath, size_t len, char **config_buf, 
    size_t bufsize);

// don't forget free the memory
static int
config_loadfile(char *filepath, size_t len, char **config_buf, 
    size_t bufsize)
{
    struct stat statbuf;

    stat(filepath, &statbuf);
    
    return CONF_OK;
}

int config_parser(char *filepath, size_t len)
{
    char   *fbuf = NULL;
    size_t  bufsize = 0; 
    int     ret;
    
    ret = config_loadfile(filepath, len, &fbuf, &bufsize);
    if (ret != CONF_OK) {
        return ret;
    }
    
    return CONF_OK;
}
