#ifndef H_SOCKET_INCLUDED
#define H_SOCKET_INCLUDED

// TODO : 1 connection design
#include "persist.h"

typedef struct listening_s              listening_t;
typedef listening_t *                   listening_p;

typedef struct connection_s             connection_t;
typedef connection_t *                  connection_p;

//  listen
struct listening_s {
    int                    fd;
    struct sockaddr        sockaddr;
    socklen_t              socklen;
};

//  conn
struct connection_s {
    int                    fd;
    struct sockaddr        sockaddr;
    socklen_t              socklen;
    void *                 read;
    void *                 write;
};


listening_p server_tcp_create(uint16_t, char *);
connection_p server_tcp_accept(listening_p);
#endif
