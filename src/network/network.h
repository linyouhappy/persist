#ifndef H_NETWORK_INCLUDED
#define H_NETWORK_INCLUDED

#include "core/core.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct listening_s              listening_t;
typedef listening_t *                   listening_p;

typedef struct connection_s             connection_t;
typedef connection_t *                  connection_p;

#include "network/event.h"
#include "network/server.h"


//  listen
struct listening_s {
    int                       fd;
    struct sockaddr           sockaddr;
    socklen_t                 socklen;
    connection_p              connection;
};

//  conn
struct connection_s {
    int                    fd;
    struct sockaddr        sockaddr;
    socklen_t              socklen;

    listening_p            listening;

    void *                 read;
    void *                 write;
};

#endif
