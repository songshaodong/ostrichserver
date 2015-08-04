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

#ifndef _ARRAYS_H_
#define _ARRAYS_H_

#include "common.h"

static inline void *realloc_down(void *ptr, size_t size)
{
	void *ret = os_realloc(ptr, size);
	return ret ? ret : ptr;
}

#define INSERT_ELEM( p_ar, i_oldsize, i_pos, elem )                            \
    do {                                                                       \
        if(!(i_oldsize)) {                                                     \
            (p_ar) = NULL;                                                     \
        }                                                                      \
        (p_ar) = os_realloc(p_ar, ((i_oldsize) + 1) * sizeof(*(p_ar)));       \
        if((i_oldsize) - (i_pos)) {                                            \
            memmove((p_ar) + (i_pos) + 1, (p_ar) + (i_pos),                   \
                ((i_oldsize) - (i_pos)) * sizeof(*(p_ar)));                   \
        }                                                                      \
        (p_ar)[(i_pos)] = elem;                                                \
        (i_oldsize)++;                                                         \
    } while( 0 )

#define REMOVE_ELEM( p_ar, i_size, i_pos )                                     \
    do {                                                                       \
        if((i_size) - (i_pos) - 1) {                                           \
            memmove((p_ar) + (i_pos), (p_ar) + (i_pos) + 1,                   \
                     ((i_size) - (i_pos) - 1) * sizeof(*(p_ar)));             \
        }                                                                      \
        if( i_size > 1 ) {                                                     \
            (p_ar) = realloc_down( p_ar, ((i_size) - 1) * sizeof( *(p_ar) ) );\
        } else {                                                               \
            os_free( p_ar );                                                   \
            (p_ar) = NULL;                                                     \
        }                                                                      \
        (i_size)--;                                                            \
    } while( 0 )

#define TAB_INIT( count, tab )                              \
            do {                                            \
                (count) = 0;                                \
                (tab) = NULL;                               \
            } while(0)

#define TAB_CLEAN( count, tab )                             \
            do {                                            \
                os_free( tab );                             \
                (count)= 0;                                 \
                (tab)= NULL;                                \
            } while(0)

#define TAB_APPEND_CAST( cast, count, tab, p )  \
  do {                                          \
    if((count) > 0)                           \
        (tab) = cast os_realloc( tab, sizeof( void ** ) * ( (count) + 1 ) ); \
    else                                        \
        (tab) = cast os_malloc( sizeof( void ** ) );    \
    (tab)[count] = (p);                         \
    (count)++;                                  \
  } while(0)

#define TAB_APPEND( count, tab, p )             \
    TAB_APPEND_CAST( , count, tab, p )

#define TAB_FIND( count, tab, p, index )        \
  do {                                          \
    (index) = -1;                               \
    for( int i = 0; i < (count); i++ )          \
        if( (tab)[i] == (p) )                   \
        {                                       \
            (index) = i;                        \
            break;                              \
        }                                       \
  } while(0)

#define TAB_REMOVE( count, tab, p )             \
  do {                                          \
        int i_index;                            \
        TAB_FIND( count, tab, p, i_index );     \
        if( i_index >= 0 )                      \
        {                                       \
            if( (count) > 1 )                   \
            {                                   \
                memmove( ((void**)(tab) + i_index),    \
                         ((void**)(tab) + i_index+1),  \
                         ( (count) - i_index - 1 ) * sizeof( void* ) );\
            }                                   \
            (count)--;                          \
            if( (count) == 0 )                  \
            {                                   \
                os_free( tab );                    \
                (tab) = NULL;                   \
            }                                   \
        }                                       \
  } while(0)

#define TAB_INSERT_CAST( cast, count, tab, p, index ) do { \
    if( (count) > 0 )                           \
        (tab) = cast os_realloc( tab, sizeof( void ** ) * ( (count) + 1 ) ); \
    else                                        \
        (tab) = cast os_malloc( sizeof( void ** ) );       \
    if( !(tab) ) abort();                       \
    if( (count) - (index) > 0 )                 \
        memmove( (void**)(tab) + (index) + 1,   \
                 (void**)(tab) + (index),       \
                 ((count) - (index)) * sizeof(*(tab)) );\
    (tab)[(index)] = (p);                       \
    (count)++;                                  \
} while(0)

#define TAB_INSERT( count, tab, p, index )      \
    TAB_INSERT_CAST( , count, tab, p, index )

#define BSEARCH( entries, count, elem, zetype, key, answer ) \
   do {  \
    int low = 0, high = count - 1;   \
    answer = -1; \
    while( low <= high ) {\
        int mid = (low + high ) / 2;  \
        zetype mid_val = entries[mid] elem;\
        if( mid_val < key ) \
            low = mid + 1; \
        else if ( mid_val > key ) \
            high = mid -1;  \
        else    \
        {   \
            answer = mid;  break;   \
        }\
    } \
 } while(0)

#define _ARRAY_ALLOC(array, newsize) {                                      \
    (array).i_alloc = newsize;                                              \
    (array).p_elems = os_realloc( (array).p_elems, (array).i_alloc *    \
                                    sizeof(*(array).p_elems) );             \
    if( !(array).p_elems ) abort();                                         \
}

#define _ARRAY_GROW1(array) {                                               \
    if( (array).i_alloc < 10 )                                              \
        _ARRAY_ALLOC(array, 10 )                                            \
    else if( (array).i_alloc == (array).i_size )                            \
        _ARRAY_ALLOC(array, (int)(array.i_alloc * 1.5) )                    \
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define DECL_ARRAY(type) struct {                                           \
    int i_alloc;                                                            \
    int i_size;                                                             \
    type *p_elems;                                                          \
}

#define TYPEDEF_ARRAY(type, name) typedef DECL_ARRAY(type) name;

#define ARRAY_INIT(array)                                                   \
  do {                                                                      \
    (array).i_alloc = 0;                                                    \
    (array).i_size = 0;                                                     \
    (array).p_elems = NULL;                                                 \
  } while(0)

#define ARRAY_RESET(array)                                                  \
  do {                                                                      \
    (array).i_alloc = 0;                                                    \
    (array).i_size = 0;                                                     \
    os_free((array).p_elems);                                               \
  (array).p_elems = NULL;                                                   \
  } while(0)

#define ARRAY_APPEND(array, elem)                                           \
  do {                                                                      \
    _ARRAY_GROW1(array);                                                    \
    (array).p_elems[(array).i_size] = elem;                                 \
    (array).i_size++;                                                       \
  } while(0)

#define ARRAY_INSERT(array,elem,pos)                                        \
  do {                                                                      \
    _ARRAY_GROW1(array);                                                    \
    if( (array).i_size - pos ) {                                            \
        memmove( (array).p_elems + pos + 1, (array).p_elems + pos,          \
                 ((array).i_size-pos) * sizeof(*(array).p_elems) );         \
    }                                                                       \
    (array).p_elems[pos] = elem;                                            \
    (array).i_size++;                                                       \
  } while(0)

#define _ARRAY_SHRINK(array) {                                              \
    if( (array).i_size > 10 && (array).i_size < (int)((array).i_alloc / 1.5) ) {  \
        _ARRAY_ALLOC(array, (array).i_size + 5);                            \
    }                                                                       \
}

#define ARRAY_REMOVE(array,pos)                                             \
  do {                                                                      \
    if( (array).i_size - (pos) - 1 )                                        \
    {                                                                       \
        memmove( (array).p_elems + pos, (array).p_elems + pos + 1,          \
                 ( (array).i_size - pos - 1 ) *sizeof(*(array).p_elems) );  \
    }                                                                       \
    (array).i_size--;                                                       \
    _ARRAY_SHRINK(array);                                                   \
  } while(0)

#define ARRAY_VAL(array, pos) array.p_elems[pos]

#define ARRAY_BSEARCH(array, elem, zetype, key, answer) \
    BSEARCH( (array).p_elems, (array).i_size, elem, zetype, key, answer)

#define FOREACH_ARRAY( item, array ) { \
    int fe_idx; \
    for( fe_idx = 0 ; fe_idx < (array).i_size ; fe_idx++ ) \
    { \
        item = (array).p_elems[fe_idx];

#define FOREACH_END() } }

typedef struct array_t {
	int i_count;
	void **pp_elems;
} array_t;

static inline void array_init(array_t * p_array)
{
	memset(p_array, 0, sizeof(array_t));
}

static inline void array_clear(array_t * p_array)
{
	os_free(p_array->pp_elems);
	memset(p_array, 0, sizeof(array_t));
}

static inline array_t *array_new(void)
{
	array_t *ret = (array_t *) os_malloc(sizeof(array_t));
	if (ret) {
		array_init(ret);
	}
	return ret;
}

static inline void array_destroy(array_t * p_array)
{
	if (!p_array) {
		return;
	}
	array_clear(p_array);
	os_free(p_array);
}

static inline int array_count(array_t * p_array)
{
	return p_array->i_count;
}

static inline void *array_item_at_index(array_t * p_array,
					     int i_index)
{
	return p_array->pp_elems[i_index];
}

static inline int array_index_of_item(array_t * p_array, void *item)
{
	int i;
	for (i = 0; i < p_array->i_count; i++) {
		if (p_array->pp_elems[i] == item) {
			return i;
		}
	}
	return -1;
}

static inline void
array_insert(array_t * p_array, void *p_elem, int i_index)
{
	TAB_INSERT_CAST((void **), p_array->i_count, p_array->pp_elems, p_elem,
	    i_index);
}

static inline void array_append(array_t * p_array, void *p_elem)
{
	array_insert(p_array, p_elem, p_array->i_count);
}

static inline void array_remove(array_t * p_array, int i_index)
{
	if (i_index >= 0) {
		if (p_array->i_count > 1) {
			memmove(p_array->pp_elems + i_index,
				p_array->pp_elems + i_index + 1,
				(p_array->i_count - i_index -
				 1) * sizeof(void *));
		}
		p_array->i_count--;
		if (p_array->i_count == 0) {
			os_free(p_array->pp_elems);
			p_array->pp_elems = NULL;
		}
	}
}

static inline uint64_t DictHash(const char *psz_string, int hashsize)
{
	uint64_t i_hash = 0;
    
	if (psz_string) {
		while (*psz_string) {
			i_hash += *psz_string++;
			i_hash += i_hash << 10;
			i_hash ^= i_hash >> 8;
		}
	}
    
	return i_hash % hashsize;
}

typedef struct dictionary_entry_t {
	char *psz_key;
	void *p_value;
	struct dictionary_entry_t *p_next;
} dictionary_entry_t;

typedef struct dictionary_t {
	int i_size;
	dictionary_entry_t **p_entries;
} dictionary_t;

static void *const kIMAXDictionaryNotFound = NULL;

static inline void dictionary_init(dictionary_t * p_dict, int i_size)
{
	p_dict->p_entries = NULL;

	if (i_size > 0) {
        
		p_dict->p_entries = (dictionary_entry_t **) calloc(i_size,
		    sizeof(*p_dict->p_entries));
        
		if (!p_dict->p_entries) {
			i_size = 0;
		}
	}
    
	p_dict->i_size = i_size;
}

static inline void dictionary_clear(dictionary_t * p_dict,
    void (*pf_os_free)(void *p_data, void *p_obj), void *p_obj)
{	
	int i;

	if (p_dict->p_entries) {
        
		for (i = 0; i < p_dict->i_size; i++) {
            
			dictionary_entry_t *p_current, *p_next;
            
			p_current = p_dict->p_entries[i];
            
			while (p_current) {
                
				p_next = p_current->p_next;
                
				if (pf_os_free != NULL) {
					(*pf_os_free) (p_current->p_value, p_obj);
				}
                
				os_free(p_current->psz_key);
				os_free(p_current);
                
				p_current = p_next;
			}
		}
        
		os_free(p_dict->p_entries);
        
		p_dict->p_entries = NULL;
        
	}
    
	p_dict->i_size = 0;
}

static inline int
dictionary_has_key(const dictionary_t * p_dict, const char *psz_key)
{
	if (!p_dict->p_entries) {
		return 0;
	}

	int i_pos = DictHash(psz_key, p_dict->i_size);
    
	return p_dict->p_entries[i_pos] != NULL;
}

static inline void *dictionary_value_for_key(const dictionary_t *
						  p_dict, const char *psz_key)
{
	if (!p_dict->p_entries) {
		return kIMAXDictionaryNotFound;
	}

	int i_pos = DictHash(psz_key, p_dict->i_size);
	dictionary_entry_t *p_entry = p_dict->p_entries[i_pos];

	if (!p_entry) {
		return kIMAXDictionaryNotFound;
	}

	do {
		if (!strcmp(psz_key, p_entry->psz_key)) {
			return p_entry->p_value;
		}
		p_entry = p_entry->p_next;
	} while (p_entry);

	return kIMAXDictionaryNotFound;
}

static inline int dictionary_keys_count(const dictionary_t * p_dict)
{
	dictionary_entry_t *p_entry;
	int i, count = 0;

	if (!p_dict->p_entries) {
		return 0;
	}

	for (i = 0; i < p_dict->i_size; i++) {
        
		for (p_entry = p_dict->p_entries[i]; p_entry; p_entry = p_entry->p_next) {
			count++;
		}
        
	}
    
	return count;
}

static inline char **dictionary_all_keys(const dictionary_t * p_dict)
{
	dictionary_entry_t *p_entry;
	char **ppsz_ret;
	int i, count = dictionary_keys_count(p_dict);

	ppsz_ret = (char **)os_malloc(sizeof(char *) * (count + 1));

	count = 0;
    
	for (i = 0; i < p_dict->i_size; i++) {
        
		for (p_entry = p_dict->p_entries[i]; p_entry; p_entry = p_entry->p_next) {
			ppsz_ret[count++] = strdup(p_entry->psz_key);
		}
	}
    
	ppsz_ret[count] = NULL;
    
	return ppsz_ret;
}

static inline void
__dictionary_insert(dictionary_t * p_dict, const char *psz_key,
    void *p_value, bool rebuild)
{
	if (!p_dict->p_entries) {
		dictionary_init(p_dict, 1);
	}
  
	int i_pos = DictHash(psz_key, p_dict->i_size);
	dictionary_entry_t *p_entry;

	p_entry = (dictionary_entry_t *) os_malloc(sizeof(*p_entry));
	p_entry->psz_key = strdup(psz_key);
	p_entry->p_value = p_value;
	p_entry->p_next = p_dict->p_entries[i_pos];
	p_dict->p_entries[i_pos] = p_entry;
    
	if (rebuild) {

		int count;
        
		for (count = 1; p_entry->p_next; count++) {
			p_entry = p_entry->p_next;
		}
        
		if (count > 3) {

			struct dictionary_t new_dict;
            
			int i_new_size = ((p_dict->i_size + 2) * 3) / 2;
            
			int i;
            
			dictionary_init(&new_dict, i_new_size);
            
			for (i = 0; i < p_dict->i_size; i++) {
                
				p_entry = p_dict->p_entries[i];
                
				while (p_entry) {
					__dictionary_insert(&new_dict,p_entry->psz_key,
					    p_entry->p_value, false);
					p_entry = p_entry->p_next;
				}
			}

			dictionary_clear(p_dict, NULL, NULL);
            
			p_dict->i_size = new_dict.i_size;
            
			p_dict->p_entries = new_dict.p_entries;
            
		}
	}
}

static inline void
dictionary_insert(dictionary_t * p_dict, const char *psz_key,
    void *p_value)
{
	__dictionary_insert(p_dict, psz_key, p_value, true);
}

static inline void
dictionary_remove_value_for_key(const dictionary_t * p_dict,
    const char *psz_key, void (*pf_os_free) (void *p_data, void *p_obj), 
    void *p_obj)
{
	if (!p_dict->p_entries) {
		return;
	}

	int i_pos = DictHash(psz_key, p_dict->i_size);
	dictionary_entry_t *p_entry = p_dict->p_entries[i_pos];
	dictionary_entry_t *p_prev;

	if (!p_entry) {
		return;
	}

	p_prev = NULL;
    
	do {
		if (!strcmp(psz_key, p_entry->psz_key)) {
            
			if (pf_os_free != NULL) {
				(*pf_os_free) (p_entry->p_value, p_obj);
			}
            
			if (!p_prev) {
				p_dict->p_entries[i_pos] = p_entry->p_next;
			} else {
				p_prev->p_next = p_entry->p_next;
			}
            
			os_free(p_entry->psz_key);
			os_free(p_entry);
            
			return;
		}
        
		p_prev = p_entry;
		p_entry = p_entry->p_next;
        
	} while (p_entry);

}

#endif
