#ifndef H_SERVER_INCLUDED
#define H_SERVER_INCLUDED

#include "network/network.h"

#define EVENT_TYPE_LISTENING            1
#define EVENT_TYPE_CONNECTION           2

typedef struct server_s                 server_t;
typedef server_t *                      server_p;


typedef struct server_event_s           server_event_t;
typedef server_event_t *                server_event_p;

typedef void (*server_read_pt)(connection_p);
typedef void (*server_close_pt)(connection_p);

event_process_pt                        server_accept;
event_process_pt                        server_read;
event_process_pt                        server_write;

server_read_pt                          server_user_read;
server_close_pt                         server_user_close;

server_p server_initialize();
int server_tcp_create(server_p);
int server_tcp_process();

void server_tcp_accept(event_p ev);
void server_tcp_write(event_p ev);
void server_tcp_read(event_p ev);

#endif
