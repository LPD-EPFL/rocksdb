/*
 * File:
 *   skiplist-lock.c
 * Author(s):
 *   Vasileios Trigonakis
 * Description:
 *   Skip list implementation of an integer set
 *
 */

#if SL == 1

#include "skiplist-lock.h"
#include "db/synchrobench-utils/utils.h"

unsigned int levelmax;
unsigned int size_pad_32;
__thread ssmem_allocator_t* alloc;

int
get_rand_level()
{
  int i, level = 1;
  for (i = 0; i < levelmax - 1; i++) 
    {
      if ((rand_range(100)-1) < 50)
	level++;
      else
	break;
    }
  /* 1 <= level <= levelmax */
  return level;
}

int
floor_log_2(unsigned int n) 
{
  int pos = 0;
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return ((n == 0) ? (-1) : pos);
}

/* 
 * Create a new node without setting its next fields. 
 */
sl_node_t*
sl_new_simple_node(strkey_t key, strval_t val, int toplevel, int transactional)
{
  sl_node_t* node;
	
#if GC == 1
  #pragma message "GC should be working \n\n\n\n\n\n\n"
  if (unlikely(transactional))
    {
      /* use levelmax instead of toplevel in order to be able to use the ssalloc allocator */
      size_t ns = size_pad_32;
      size_t ns_rm = ns & 63;
      if (ns_rm)
	{
	  ns += 64 - ns_rm;
	}
      node = (sl_node_t*)ssalloc_aligned(CACHE_LINE_SIZE, ns);
    }
  else 
    {
      size_t ns = size_pad_32;
#  if defined(DO_PAD)
      size_t ns_rm = ns & 63;
      if (ns_rm)
	{
	  ns += 64 - ns_rm;
	}
#  endif
      node = (sl_node_t*) ssmem_alloc(alloc, ns);
    }
#else
  size_t ns = size_pad_32;
  if (transactional)
    {
      size_t ns_rm = ns & 63;
      if (ns_rm)
	{
	  ns += 64 - ns_rm;
	}
    }
  node = (sl_node_t*)ssalloc(ns);
#endif

  node->key = key;
  node->val = val;
  node->toplevel = toplevel;
  INIT_LOCK(ND_GET_LOCK(node));

#if defined(__tile__)
  MEM_BARRIER;
#endif

  return node;
}

/* 
 * Create a new node with its next field. 
 * If next=NULL, then this create a tail node. 
 */
sl_node_t*
sl_new_node(strkey_t key, strval_t val, sl_node_t *next, int toplevel, int transactional)
{
  sl_node_t *node;
  int i;
	
  node = sl_new_simple_node(key, val, toplevel, transactional);
	
  for (i = 0; i < toplevel; i++)
    node->next[i] = next;
	
  MEM_BARRIER;

  return node;
}

void
sl_delete_node(sl_node_t *n)
{
  DESTROY_LOCK(ND_GET_LOCK(n));
  ssfree_alloc(1, (void*) n);
}

sl_intset_t*
sl_set_new()
{
  sl_intset_t *set;
  sl_node_t *min, *max;
	
  if ((set = (sl_intset_t *)ssalloc_aligned(CACHE_LINE_SIZE, sizeof(sl_intset_t))) == NULL)
    {
      perror("malloc");
      exit(1);
    }

  printf("LEVEL MAX: %d\n", levelmax);  
  int64_t m = ALT_KEY_MIN;
  int64_t M = ALT_KEY_MAX;
  strkey_t key_min, key_max;
  int64_to_char8(key_min.key, m);
  int64_to_char8(key_max.key, M);

  strval_t val_0 = {""};

  max = sl_new_node(key_max, val_0, NULL, levelmax, 1);
  min = sl_new_node(key_min, val_0, max, levelmax, 1);
  set->head = min;

#if defined(LL_GLOBAL_LOCK)
  set->lock = (volatile ptlock_t*) ssalloc_aligned(CACHE_LINE_SIZE, sizeof(ptlock_t));
  if (set->lock == NULL)
    {
      perror("malloc");
      exit(1);
    }
  GL_INIT_LOCK(set->lock);
#endif

  return set;
}

void
sl_set_delete(sl_intset_t *set)
{
  sl_node_t *node, *next;
	
  node = set->head;
  while (node != NULL)
    {
      next = node->next[0];
      sl_delete_node(node);
      node = next;
    }
#if defined(LL_GLOBAL_LOCK)
  ssfree((void*) set->lock);
#endif
  ssfree((void*) set);
}

int sl_set_size(sl_intset_t *set)
{
  int size = 0;
  sl_node_t *node;
	
  /* We have at least 2 elements */
  node = set->head->next[0];
  while (node->next[0] != NULL) 
    {
      size++;
      node = node->next[0];
    }
  return size;
}

#endif
