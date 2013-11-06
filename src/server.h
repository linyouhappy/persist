#ifndef SERVER_H_
#define SERVER_H_

#include "event/event.h"

typedef struct server_s                 server_t;
typedef server_t *                      server_p;

struct server_s {
    char                     *ip;
    uint16_t                  port;

    event_process_pt          accept;
    event_process_pt          read;
    event_process_pt          write;
};

#define EVENT_TYPE_LISTENING            1
#define EVENT_TYPE_CONNECTION           2

typedef struct listening_s              listening_t;
typedef listening_t *                   listening_p;

typedef struct connection_s             connection_t;
typedef connection_t *                  connection_p;

typedef struct server_event_s           server_event_t;
typedef server_event_t *                server_event_p;


event_process_pt                        server_read;
event_process_pt                        server_write;


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

int server_tcp_create(server_p);
int server_tcp_process();

#endif /* SERVER_H_ */
