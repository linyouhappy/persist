#ifndef H_PERSIST_INCLUDED
#define H_PERSIST_INCLUDED

#define VERSION         0.1

#define null            NULL
#define true            1
#define false           0

#define SUCCESS         EXIT_SUCCESS
#define FAILED          EXIT_FAILED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "queue.h"
#include "thread.h"             //  线程 && 线程池
#include "connection.h"         //  连接

#endif
