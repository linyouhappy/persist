#ifndef H_CORE
#define H_CORE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // memset
#include <sys/types.h>  // u_char

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

#include "core/string.h"
#include "core/array.h"
#include "core/hash.h"

void core_init(void);
void core_cpuinfo(void);

#endif
