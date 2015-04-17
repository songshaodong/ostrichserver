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
#include <common.h>

// base on Blizzard _One-Way Hash_, http://www.programering.com/a/MDOyADMwATE.html

int crypttable[0x500];

int init_crypttable()
{
    uint32_t  seed = 0x00100001;
    uint32_t  index1 = 0;
    uint32_t  index2 = 0;
    uint32_t  i;

    for (index1 = 0; index1 < 0x100; index1++) {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {
            uint32_t  t1;
            uint32_t  t2;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            t1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            t2 = (seed & 0xFFFF);
            crypttable[index2] = (t1 | t2);
        }
    }
}

int hashstring(char *string, int hashtype)
{
    unsigned char *key = (unsigned char *)string;
    unsigned char *seed1 = 0x7FED7FED;
    unsigned char *seed2 = 0xEEEEEEEE;

    int ch;

    while (*key != 0) {
        ch = toupper(*key++);
        seed1 = crypttable[(hashtype << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }

    return seed1;
}


int hashtable_init(char **pphashtable, int ntablelength)
{
    int        i = 0;
    char      *p = NULL;
    hashtable *ptable = NULL;

    init_crypttable();

    p = (hashtable *)os_malloc(ntablelength * sizeof(hashtable);
    if (p = NULL) {
        return 0;
    }

    *pphashtable = p;
    ptable = p;

    for (i = 0; i < ntablelength; i++) {
        (ptable + i)->n_hashA = -1;
        (ptable + i)->n_hashB = -1;
        (ptable + i)->exists = 0;
    }

    return 1;
}

void hashtable_free(char *phashtable) {
    if (phashtable != NULL) {
        os_free(phashtable);
    }
}

int hashtable_add(char *string, char *phashtable)
{
    uint32_t hashoffset = 0;
    uint32_t hashA = 1;
    uint32_t hashB = 2;

    uint32_t nhash = hashstring(string, hashoffset);
    uint32_t nhashA = hashstring(string, hashA);
    uint32_t nhashB = hashstring(string, hashB);
    uint32_t nhashstart = nhash % MAXHASHTABLELEN;
    uint32_t nhashpos = nhashstart;
    hashtable *_phtable = (hashtable *)phashtable;

    while ((_phtable + nhashpos)->exists) {
        nhashpos = (nhashpos + 1) % MAXHASHTABLELEN;
        if (nhashpos == nhashstart) {
            return 0;
        }
    }

    (_phtable + nhashpos)->nhashA = nhashA;
    (_phtable + nhashpos)->nhashB = nhashB;
    (_phtable + nhashpos)->exists = 1;

    return 1;
}

int hashtable_isexist(char *string, char *phashtable)
{
    uint32_t hashoffset = 0;
    uint32_t hashA = 1;
    uint32_t hashB = 2;

    uint32_t nhash = hashstring(string, hashoffset);
    uint32_t nhashA = hashstring(string, hashA);
    uint32_t nhashB = hashstring(string, hashB);
    uint32_t nhashstart = nhash % MAXHASHTABLELEN;
    uint32_t nhashpos = nhashstart;
    hashtable *_phtable = (hashtable *)phashtable;

    while ((_phtable + nhashpos)->exist) {
        if (((_phtable + nhashpos)->nhashA == nhashA) &&
            ((_phtable + nhashpos)->nhashB == nhashB)) {
            return nhashpos;
        } else {
            nhashpos = (nhashpos + 1) % MAXHASHTABLELEN;
        }

        if (nhashpos == nhashstart) {
            break;
        }
    }

    return -1;
    
}
