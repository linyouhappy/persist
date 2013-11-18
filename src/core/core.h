#ifndef H_CORE_INCLUDED
#define H_CORE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // memset
#include <unistd.h>     // read

#include <sys/types.h>  // u_char

#include <sys/errno.h>

#define __CORE_VERSION          0.1

#define null                    NULL
#define true                    1
#define false                   0

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS            0
#endif

#ifndef EXIT_FAILED
#define EXIT_FAILED             1
#endif

#define SUCCESS                 EXIT_SUCCESS
#define FAILED                  EXIT_FAILED

#define success                 SUCCESS
#define failed                  FAILED

// type
//typedef __int8_t                int_8;
//typedef __uint8_t               uint_8;
//typedef short                   int16_t;
//typedef unsigned short          __uint16_t;
//typedef int                     __int32_t;
//typedef unsigned int            uint;
//typedef long long               __int64_t;
//typedef unsigned long long      __uint64_t;

#include "core/string.h"
#include "core/mpm.h"
#include "core/array.h"
#include "core/hash.h"

void core_init(void);
void core_cpuinfo(void);

#endif
