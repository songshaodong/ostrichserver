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

#include <hashtable.h>
#include <config.h>

hashtable config_record_hashtable;

int config_hashtable_init()
{
    int result;
    
    config_record_hashtable.size = MAXHASHTABLELEN;
    
    result = hashtable_init(config_record_hashtable.buckets, 
        config_record_hashtable.size);
    
    if (!result) {
        return OS_ERR;
    }

    return OS_OK;
}

int get_filesize(int fd)
{
  struct stat stats;
  
  fstat(fd, &stats);
  
  return (int) stats.st_size;
}

int config_init(char *path, char **fbuf)
{
    int   filesize;
    int   fd = -1;
    char *pbuf = NULL;
    int   result =0;

    filesize = strlen(path);

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        return OS_ERR;
    }
    
    filesize = get_filesize(fd);

    pbuf = os_malloc(filesize + 1);
    if (fbuf == NULL) {
        return OS_ERR;
    }
    
    result = read(fd, pbuf, filesize);
    if (result < 0) {
        return result;
    }

    pbuf[filesize] = '\0';

    *fbuf = pbuf;

    return OS_OK;
}


int config_parse_file(char *path)
{
    int result = 0;
    
    char *pbuf = NULL;
    char *line = NULL;
    char *name = NULL;
    char *next = NULL;
    
    result = config_init(path, &pbuf);

    if (result < 0) {
        return OS_ERR;
    }

    line = strtok_r(pbuf, "\r\n", &next);
    if (line == NULL) {
        return OS_ERR; // empty config file
    }
    
    while (*next != '\0') {
        name = strtok_r(NULL, " \t", &next);
        if (name == NULL) {
        }
    }
}

