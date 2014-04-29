#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>

#include "getticks.h"
#include "latency.h"
#include "barrier.h"
#include "main_test_loop.h"

#define XSTR(s)                         STR(s)
#define STR(s)                          #s

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#  define STATIC_ASSERT(a, msg)           _Static_assert ((a), msg);
#else 
#  define STATIC_ASSERT(a, msg)           
#endif

typedef intptr_t skey_t;
typedef intptr_t sval_t;

#define KEY_MIN                         INT_MIN
#define KEY_MAX                         (INT_MAX - 2)

#define DEFAULT_DURATION                1000
#define DEFAULT_INITIAL                 1024
#define DEFAULT_NB_THREADS              1
#define DEFAULT_RANGE                   (2 * DEFAULT_INITIAL)
#define DEFAULT_UPDATE                  20

#ifdef __cplusplus
}
#endif

#endif	/*  _COMMON_H_ */
