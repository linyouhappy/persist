#ifndef H_SERVER_INCLUDED
#define H_SERVER_INCLUDED

#define EVENT_TYPE_LISTENING            1
#define EVENT_TYPE_CONNECTION           2

typedef struct server_s                 server_t;
typedef server_t *                      server_p;


typedef struct server_event_s           server_event_t;
typedef server_event_t *                server_event_p;

#include "network/network.h"

typedef void (*server_accept_pt)(connection_p);
typedef void (*server_read_pt)(connection_p, uint);
typedef void (*server_close_pt)(connection_p);


struct server_s {
    char                     *ip;
    uint16_t                  port;

    server_accept_pt          accept;   //  @TODO ACCEPT 接口
    server_read_pt            read;
    server_close_pt           close;

//    void                      write;    //  @TODO 不晓得啥时候触发，暂时用不上
};

event_process_pt                        server_accept;
event_process_pt                        server_read;
event_process_pt                        server_write;

server_accept_pt                        server_user_accept;
server_read_pt                          server_user_read;
server_close_pt                         server_user_close;


//  初始化
server_p server_init();
int server_tcp_create(server_p);
int server_tcp_process();

void server_tcp_accept(event_p ev);
void server_tcp_write(event_p ev);
void server_tcp_read(event_p ev);

#endif
