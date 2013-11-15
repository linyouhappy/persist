#include "demo/server.h"

static void server_demo_accept(connection_p conn) {
    printf("[user]%d:accept\n", conn->fd);
}

static void server_demo_read(connection_p conn) {
    printf("[user]%d:read\n", conn->fd);
}

static void server_demo_close(connection_p conn) {
    printf("[user]%d:close\n", conn->fd);
}

void server_demo_example(void) {
    server_p server;

    server = server_init();

    server->port = 57717;

    server->accept = server_demo_accept;
    server->read  = server_demo_read;
    server->close = server_demo_close;

    server_tcp_create(server);

    server_tcp_process();

}
