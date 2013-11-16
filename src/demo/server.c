#include "demo/server.h"

static void server_demo_accept(connection_p conn) {
    printf("[user]%d:accept\n", conn->fd);
}

static void server_demo_read(connection_p conn, uint size) {
    int  fd;
    uint n;
    fd = conn->fd;

    char buff[1024];
    memset(buff, 0, 1024);

    if (-1 == (n = read(fd, buff, 1024))) {
        printf("read failed!(%s)\n", strerror(errno));
        exit(1);
    } else {
        printf("%d:read message\n%s\n", fd, buff);
    }

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

    if (failed == server_tcp_create(server)) {
        printf("create server failed!\n");
        exit(1);
    }
    printf("server(%s:%d) create success!\n", (null == server->ip) ? "127.0.0.1" : server->ip, server->port);

    server_tcp_process();

}
