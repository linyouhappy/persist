#ifndef H_PERSIST_INCLUDED
#define H_PERSIST_INCLUDED

#define VERSION         0.1

#define null            NULL
#define true            1
#define false           0

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILED
#define EXIT_FAILED  1
#endif

#define SUCCESS         EXIT_SUCCESS
#define FAILED          EXIT_FAILED

#define success         SUCCESS
#define failed          FAILED

//  system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

//  persist base
#include "base/string.h"
#include "base/array.h"
#include "base/hash.h"

#include "queue.h"
#include "thread.h"             //  线程 && 线程池

#include "event/event.h"
#include "server.h"

#endif
