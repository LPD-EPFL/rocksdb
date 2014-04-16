#ifndef _COMMON_H_
#define _COMMON_H_
#include <limits.h>
#include <string>
#include <string.h>

#include "getticks.h"
#include "latency.h"
#include "barrier.h"
#include "main_test_loop.h"

#define XSTR(s)                         STR(s)
#define STR(s)                          #s

typedef const char* skey_t;
typedef const char* sval_t;

#define DEFAULT_DURATION                1000
#define DEFAULT_INITIAL                 1024
#define DEFAULT_NB_THREADS              1
#define DEFAULT_RANGE                   (2 * DEFAULT_INITIAL)
#define DEFAULT_UPDATE                  20

inline const char* KEY_MAX() {
	std::string str = "";
	str.push_back(CHAR_MAX);
	return str.data();
}

inline const char* KEY_MIN() {
	std::string str = "";
	str.push_back(CHAR_MIN);
	return str.data();
}

inline int skey_compare(skey_t k1, skey_t k2) {
	return strcmp(k1, k2);
}

#endif	/*  _COMMON_H_ */
