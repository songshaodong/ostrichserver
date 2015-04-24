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
#include "config.h"
#include <string.h>

hashtable *config_record_hashtable;

#define iter_to_next_line(curline, nextline)         \
        do {                                         \
            curline = nextline;                      \
            strtok_r(NULL, "\n", &nextline);         \
        } while (0);

int config_hashtable_init(int size)
{
    int result;

    (void) size;

    config_record_hashtable = os_calloc(sizeof(hashtable));
    config_record_hashtable->size = MAXHASHTABLELEN;
    
    result = hashtable_init(&config_record_hashtable->buckets, 
        config_record_hashtable->size);
    
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

int record_make(record **rc, char *name, char *type, char *value)
{
    record  *pr;

    pr = os_calloc(sizeof(record));

    pr->name = os_strdup(name);
    
    if (!strcmp(type, "STRING")) {
        pr->data.config_string = os_strdup(value);
        pr->type = RECORD_STRING;
    } else if (!strcmp(type, "INT")) {
        pr->data.config_int = os_atoi(value);
        pr->type = RECORD_INT;
    } else if (!strcmp(type, "FLOAT")) {
        pr->data.config_float = atof(value);
        pr->type = RECORD_FLOAT;
    } else {
        return OS_ERR;;
    }
    
    *rc = pr;
    
    return OS_OK;
}

int config_parse_file(char *path)
{
    int result = 0;
    
    char   *pbuf = NULL;
    char   *line = NULL;
    char   *name = NULL;
    char   *nextln = NULL;
    char   *nextblk = NULL;
    char   *type = NULL;
    char   *value = NULL;
    record *rc = NULL;

    config_hashtable_init(1);
    
    result = config_init(path, &pbuf);

    if (result < 0) {
        return OS_ERR;
    }

    line = strtok_r(pbuf, "\n", &nextln);
    
    while (*line != '\0') {
        
        while (*line == '\n') { // skip empty line and get a new line.
            line++;
        }

        if (*line == '#') {
            iter_to_next_line(line, nextln);
            continue;
        }
        
        name = strtok_r(line, " \t", &nextblk);
        
        type = strtok_r(NULL, " \t", &nextblk);

        value = strtok_r(NULL, " \t", &nextblk);

        if (hashtable_isexist(name, config_record_hashtable) != -1) {
            return OS_ERR; // duplicate config.
        }

        if (record_make(&rc, name, type, value) == OS_ERR) {
            return OS_ERR;
        }
        
        hashtable_add(name, rc, config_record_hashtable);

        if (*nextblk != '\0') { // todo: support more than tree args.
            return OS_ERR;
        }

        iter_to_next_line(line, nextln);
    }
}

inline record *get_config_record(char *str)
{
    int       index;
    hashitem *hi;
    record   *rc;

    index = hashtable_isexist(str, config_record_hashtable);
    if (index == -1) {
        return NULL;
    }
    
    hi = config_record_hashtable->buckets + index;

    rc = hi->data;

    assert(rc != NULL);

    return rc;   
}

