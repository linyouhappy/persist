#ifndef SERVER_H_
#define SERVER_H_

#include "event/event.h"

#define EVENT_TYPE_LISTENING            1
#define EVENT_TYPE_CONNECTION           2

typedef struct server_s                 server_t;
typedef server_t *                      server_p;


typedef struct listening_s              listening_t;
typedef listening_t *                   listening_p;

typedef struct connection_s             connection_t;
typedef connection_t *                  connection_p;

typedef struct server_event_s           server_event_t;
typedef server_event_t *                server_event_p;

typedef void (*server_read_pt)(connection_p);
typedef void (*server_close_pt)(connection_p);

event_process_pt                        server_accept;
event_process_pt                        server_read;
event_process_pt                        server_write;

server_read_pt                          server_user_read;
server_close_pt                         server_user_close;


struct server_s {
    char                     *ip;
    uint16_t                  port;

    server_read_pt            read;
    server_close_pt           close;

//    void                      accept;   //  @TODO ACCEPT 接口
//    void                      write;    //  @TODO 不晓得啥时候触发，暂时用不上
};


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
server_p server_initialize();
int server_tcp_create(server_p);
int server_tcp_process();

void server_tcp_accept(event_p ev);
void server_tcp_write(event_p ev);
void server_tcp_read(event_p ev);

#endif /* SERVER_H_ */
