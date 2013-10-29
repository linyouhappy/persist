#ifndef H_SOCKET_INCLUDED
#define H_SOCKET_INCLUDED

// TODO : 1 connection design
#include "persist.h"

#include <sys/epoll.h>

#define EVENT_TYPE_LISTENING            1
#define EVENT_TYPE_CONNECTION           2

typedef struct listening_s              listening_t;
typedef listening_t *                   listening_p;

typedef struct connection_s             connection_t;
typedef connection_t *                  connection_p;

typedef struct server_event_s           server_event_t;
typedef server_event_t *                server_event_p;

typedef void (*connection_process)(connection_p);

struct server_event_s {
    int     type;
    void   *ptr;
};

//  listen
struct listening_s {
    int                    fd;
    struct sockaddr        sockaddr;
    socklen_t              socklen;
    connection_process     process;
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
void server_tcp_process(listening_p);
connection_p server_tcp_accept(listening_p);

//  event poll
int epoll_add_event(int efd, int fd, struct epoll_event *);
int epoll_del_event(int efd, int fd, struct epoll_event *);
#endif
