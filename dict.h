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

#ifndef _DICT_H_
#define _DICT_H_

typedef struct dict_entry {
    void  *key;
    union {
        void      *val;
        uint64_t   u64;
        int64_t    s64;
    } v;

    struct dict_entry  *next;
} dict_entry;

typedef struct dict_func {
    int     (*hash)(void *key);
    void   *(*key_dup)(void *privdata, void *key);
    void   *(*val_dup)(void *privdata, void *key);
    int     (*key_cmp)(void *privdata, void *key1, void *key2);
    void    (*key_free)(void *privdata, void *key);
    void    (*val_free)(void *privdata, void *key);
} dict_func;

typedef struct dictht {
    dict_entry  **table;
    unsigned long size;
    unsigned long sizemask;
    unsigned long used;
} dictht; 

typedef struct dict {
    dict_func *handler;
    void      *privdata;
    dictht     ht[2];
    int        rehashidx;
    int        iterators;
} dict;

typedef struct dict_iterator {
    dict        *d;
    int          table;
    int          index;
    int          safe;

    dict_entry  *entry;
    dict_entry  *next_entry;  
} dict_iterator;

#define    DICT_HT_SIZE    11

#define dict_free_val(d, entry)                                        \
    if ((d)->handler->val_free)                                        \
        (d)->handler->val_free((d)->privdata, (entry)->v.val)

#define dict_set_val(d, entry, _val) do {                              \
        if ((d)->handler->val_dup)                                     \
            entry->v.val = (d)->handler->val_dup((d)->privdata, _val); \
        else                                                           \
            entry->v.val = (_val);                                     \
    } while (0)

#define dict_set_signint_val(entry, _val)      \
        do {                                   \
           entry->v.s64 = _val;                \
        } while (0);

#define dict_set_unsignint_val(entry, _val)    \
        do {                                   \
            entry->v.u64 = _val;               \
        } while (0);

#define dict_free_key(d, entry)                \
        if ((d)->handler->key_free)            \
            (d)->handler->key_free((d)->privdata, (entry)->key)
            
#define dict_set_key(d, entry, _key)                                     \
        do {                                                             \
            if ((d)->handler->key_dup)                                   \
                entry->key = (d)->handler->key_dup((d)->privdata, _key); \
            else                                                         \
                entry->key = (_key);                                     \
        } while (0);
           
#define dict_cmp_key(d, k1, k2)                                          \
        ((d)->handler->key_cmp) ?                                        \
            (d)->handler->key_cmp((d)->privdata, k1, k2) : (k1 == k2) 


#define dict_hash_key(d, key) (d)->handler->hash(key)
#define dict_get_key(he)   (he)->key
#define dict_get_val(he)   (he)->v.val
#define dict_get_signint_val(he) (he)->v.s64
#define dict_get_unsignint_val(he) (he)->v.u64
#define dict_slot(d)    ((d)->ht[0].size + (d)->ht[1].size)
#define dict_size(d)    ((d)->ht[0].used + (d)->ht[1].used)
#define dict_isrehashing(ht)  ((ht)->rehashidx != -1)

dict *dict_create(dict_func *func, void *privdata);
int   dict_expand(dict *d, unsigned long size);
int   dict_add(dict *d, void *key, void *val);
dict_entry *dict_add_raw(dict *d, void *key);
int   dict_replace(dict *d, void *key, void *val);
dict_entry *dict_replace_raw(dict *d, void *key);
int dict_delete(dict *d, void *key);
int dict_delete_no_free(dict *d, void *key);
void dict_release(dict *d);
dict_entry *dict_find(dict *d, void *key);
void *dict_fetch_value(dict *d, void *key);
int   dict_resize(dict *d);
void dict_empty(dict *d);
int dict_rehash(dict *d, int n);


#endif