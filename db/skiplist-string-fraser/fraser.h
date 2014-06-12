/*
 * File:
 *   fraser.h
 * Author(s):
 *   Vincent Gramoli <vincent.gramoli@epfl.ch>
 * Description:
 *   Lock-based skip list implementation of the Fraser algorithm
 *   "Practical Lock Freedom", K. Fraser, 
 *   PhD dissertation, September 2003
 *   Cambridge University Technical Report UCAM-CL-TR-579 
 *
 * Copyright (c) 2009-2010.
 *
 * fraser.h is part of Synchrobench
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
#if SL == 3

#ifdef __cplusplus
extern "C" {
#endif

#include "skiplist.h"
#include "db/synchrobench-utils/ssalloc.h"

strval_t* fraser_find(sl_intset_t *set, strkey_t key);
strval_t* fraser_remove(sl_intset_t *set, strkey_t key);
int fraser_insert(sl_intset_t *set, strkey_t key, strval_t val);

#ifdef __cplusplus
}
#endif

#endif