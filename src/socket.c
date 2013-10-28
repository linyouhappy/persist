#include "persist.h"

listening_p server_tcp_create(uint16_t port, char * ip) {
    listening_p listening;
    struct sockaddr_in * sockaddr;

    listening = (listening_p) malloc(sizeof(listening_t));
    memset(listening, 0, sizeof(listening_t));

    listening->socklen = sizeof(struct sockaddr);
    sockaddr = (struct sockaddr_in *) &listening->sockaddr;

    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port   = htons(port);
    if (null == ip) {
        sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    }

    listening->fd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == bind(listening->fd, (struct sockaddr *)sockaddr, sizeof(struct sockaddr))) {
        printf("bind failed!(%s)\n", strerror(errno));
        exit(0);
    }

    if (-1 == listen(listening->fd, 10)) {
        printf("listen failed!\n");
    }

    printf("server create true!\n");
    return listening;
}

connection_p server_tcp_accept(listening_p listen) {
    int                  socklen;
    struct sockaddr_in * sockaddr;
    socklen  = sizeof(struct sockaddr_in);
    sockaddr = (struct sockaddr_in *) malloc(socklen);

    connection_p connection;
    connection = (connection_p) malloc(sizeof(connection_t));
    memset(connection, 0, sizeof(listening_t));

    connection->socklen  = sizeof(struct sockaddr_in);

    connection->fd = accept(listen->fd, (struct sockaddr *)&connection->sockaddr, &connection->socklen);

    return connection;
}
