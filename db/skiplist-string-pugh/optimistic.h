/*
 * File:
 *   optimistic.h
 * Author(s):
 *   Vincent Gramoli <vincent.gramoli@epfl.ch>
 * Description:
 *   Fine-grained locking skip list implementation of Herlihy et al. algorithm
 *   "A Simple Optimistic Skiplist Algorithm" 
 *   M. Herlihy, Y. Lev, V. Luchangco, N. Shavit 
 *   p.124-138, SIROCCO 2007
 *
 * Copyright (c) 2009-2010.
 *
 * optimistic.h is part of Synchrobench
 * 
 * Synchrobench is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#if SL == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "skiplist-lock.h"

strval_t* optimistic_find(sl_intset_t *set, strkey_t key);
int optimistic_insert(sl_intset_t *set, strkey_t key, strval_t val);
strval_t* optimistic_delete(sl_intset_t *set, strkey_t key);

#ifdef __cplusplus
}
#endif

#endif