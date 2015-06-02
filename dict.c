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
#include "dict.h"

#define DICT_RESIZE_RATIO    5  

uint32_t dict_hash_function_seed = 5381;

/* MurmurHash2, by Austin Appleby. */
unsigned int
dict_gen_key(void * key, int len) {

    uint32_t seed = dict_hash_function_seed;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    uint32_t h = seed ^ len;

    const unsigned char *data = (const unsigned char *)key;

    while(len >= 4) {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0]; h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int)h;
}

unsigned int 
dict_casestring_key(const unsigned char *buf, int len) {

    unsigned int hash = (unsigned int)dict_hash_function_seed;

    while (len--)
        hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
    return hash;
}

unsigned int dict_int_key(unsigned int key)
{
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}

int dict_gen_key_compare(void *privdata, void *key1, void *key2)
{
    (void) privdata;
}

int dict_string_key_compare(void *privdata, void *key1, void *key2)
{
    (void) privdata;

    string *k1 = key1;
    string *k2 = key2;

    if (k1->len != k2->len) {
        return -1;
    }

    return strncmp(k1->data, k2->data, k1->len);
}

void _dict_reset(dictht *ht)
{
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}

int _dict_init(dict *d, dict_type *f, void *privdata)
{
    _dict_reset(&d->ht[0]);
    _dict_reset(&d->ht[1]);

    d->type = f;
    d->privdata = privdata;
    d->rehashidx = -1;
    d->iterators = 0;
}

unsigned long _dict_next_power(unsigned long size)
{
    unsigned long i = DICT_HT_SIZE;

    if (size >= LONG_MAX) {
        return LONG_MAX;
    } 

    for (;;) {
        if (i >= size) {
            return i;
        }

        i *= 2;
    }
}

dict *dict_create(dict_type *func, void *privdata)
{
    dict *d = os_malloc(sizeof(dict));

    _dict_init(d, func, privdata);

    return d;
}

int dict_expand(dict *d, unsigned long size)
{
    dictht n;

    unsigned long realsize = _dict_next_power(size);

    if (dict_isrehashing(d) || d->ht[0].used > size) {
        return OS_ERR;
    }

    n.size = realsize;
    n.sizemask = realsize - 1;
    n.table = os_calloc(realsize * sizeof(dict_entry *));
    n.used = 0;

    if (d->ht[0].table == NULL) {
        d->ht[0] = n;
        return OS_OK;
    }

    d->ht[1] = n;
    d->rehashidx = 0;

    return OS_OK;
}

void _dict_rehash_step(dict *d)
{
    if (d->iterators == 0) {
        dict_rehash(d, 1);
    }
}

int dict_rehash(dict *d, int n)
{
    if (!dict_isrehashing(d)) {
        return 0;
    }

    while (n--) {
        
        dict_entry  *de;
        dict_entry  *denext;

        if (d->ht[0].used == 0) {
            
            os_free(d->ht[0].table);
            d->ht[0] = d->ht[1];
            _dict_reset(&d->ht[1]);
            d->rehashidx = -1;

            return 0;
        }
        
        while (d->ht[0].size > d->rehashidx) {
            d->rehashidx++;
        }

        de = d->ht[0].table[d->rehashidx];

        while (de) {
            unsigned int h;
            denext = de->next;
            h = dict_hash_key(d, de->key) & d->ht[1].sizemask;
            de->next = d->ht[1].table[h];
            d->ht[0].used--;
            d->ht[1].used++;

            de = denext;
        }

        d->ht[0].table[d->rehashidx] = NULL;

        d->rehashidx++;
    }
    
    return 1; 
}

int _dict_expand_if_needed(dict *d)
{
    if (dict_isrehashing(d)) {
        return OS_OK;
    }

    if (d->ht[0].size == 0) {
        return dict_expand(d, DICT_HT_SIZE);
    }

    if (d->ht[0].used >= d->ht[0].size
        && d->ht[0].used / d->ht[0].size > DICT_RESIZE_RATIO) {
        return dict_expand(d, d->ht[0].used * 2);
    }

    return OS_OK;
}

int _dict_key_index(dict *d, void *key)
{
    unsigned int  h;
    unsigned int  idx;
    unsigned int  table;
    dict_entry   *he;

    if (_dict_expand_if_needed(d) == OS_ERR) {
        return -1;
    }

    h = dict_hash_key(d, key);

    for (table = 0; table <= 1; table++) {
        idx = h & d->ht[table].sizemask;

        he = d->ht[table].table[idx];

        while (he) {
            if (dict_cmp_key(d, key, he->key)) {
                return -1;
            }

            he = he->next;
        }

        if (!dict_isrehashing(d)) {
            break;
        }
    }

    return idx;
}

dict_entry *dict_add_raw(dict *d, void *key)
{
    int          index;
    dict_entry  *entry;
    dictht      *ht;

    if (dict_isrehashing(d)) {
        _dict_rehash_step(d);
    }

    if ((index = _dict_key_index(d, key)) == -1) {
        return NULL;
    }

    ht = dict_isrehashing(d) ? &d->ht[1] : &d->ht[0];

    entry = os_malloc(sizeof(*entry));

    entry->next = ht->table[index];
    
    ht->table[index] = entry;

    ht->used++;

    dict_set_key(d, entry, key);

    return entry;
}

int dict_add(dict *d, void *key, void *val)
{
    dict_entry  *entry = dict_add_raw(d, key);

    if (!entry) {
        return OS_ERR;
    }

    dict_set_val(d, entry, val);

    return OS_OK;
}

dict_entry *dict_find(dict *d, void *key)
{
    dict_entry *he;

    unsigned int h;
    unsigned int idx;
    unsigned int table;

    if (d->ht[0].size == 0) {
        return NULL;
    }

    if (dict_isrehashing(d)) {
        _dict_rehash_step(d);
    }

    h = dict_hash_key(d, key);

    for (table = 0; table <= 1; table++) {
        
        idx = h & d->ht[table].sizemask;
        
        he = d->ht[table].table[idx];

        while (he) {
            if (!dict_cmp_key(d, key, he->key)) {
                return he;
            }

            he = he->next;
        }

        if (!dict_isrehashing(d)) {
            break;
        }  
    }

    return NULL;
}

int dict_replace(dict *d, void *key, void *val)
{
    dict_entry  *entry;
    dict_entry   auxentry;

    if (dict_add(d, key, val) == OS_OK) {
        return 1;
    }

    entry = dict_find(d, key);

    auxentry = *entry;
    
    dict_set_val(d, entry, val);
    
    dict_free_val(d, &auxentry);

    return 0;
}

dict_entry  *dict_replace_raw(dict *d, void *key)
{
    dict_entry  *entry = dict_find(d, key);

    return entry ? entry : dict_add_raw(d, key);
}

int _dict_clear(dict *d, dictht *ht)
{
    unsigned long i;

    for (i = 0; i < ht->size && ht->used > 0; i++) {
        dict_entry *he;
        dict_entry *henext;

        if ((he = ht->table[i]) == NULL) {
            continue;
        }

        while (he) {
            henext = he->next;

            dict_free_key(d, he);
            dict_free_val(d, he);

            os_free(he);

            ht->used--;

            he = henext;
        }
    }

    os_free(ht->table);

    _dict_reset(ht);

    return OS_OK;
}

void dict_release(dict *d)
{
    _dict_clear(d, &d->ht[0]);
    _dict_clear(d, &d->ht[1]);

    os_free(d);
}
