#include "persist.h"

listening_p server_tcp_create(uint16_t port, char * ip) {
    listening_p listening;
    struct sockaddr_in * sockaddr;

    listening = (listening_p) malloc(sizeof(listening_t));
    memset(listening, 0, sizeof(listening_t));

    listening->process = null;

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

void server_tcp_process(listening_p listen) {
    int                efd, num, n, i;
    struct epoll_event ee, *events;
    server_event_p     se;
    listening_p        listening;
    connection_p       connection;

    num = 10;

    if (-1 == (efd = epoll_create(num))) {
        printf("epoll_create failed!(%s)", strerror(errno));
        exit(1);
    }

    se = malloc(sizeof(server_event_t));
    se->type = EVENT_TYPE_LISTENING;
    se->ptr  = listen;

    ee.data.ptr = (void *)se;
    ee.events   = 0;
    epoll_add_event(efd, listen->fd, &ee);

    events = (struct epoll_event *) malloc(num * sizeof(struct epoll_event));

    n = epoll_wait(efd, events, num, -1);

    for(i=0; i<num; i++) {
        se = (server_event_p) events[i].data.ptr;

        switch(se->type) {
        case EVENT_TYPE_LISTENING:
            listening = se->ptr;
            connection = server_tcp_accept(listen);
            if (listening->process) {
                listening->process(connection);
            }
            break;
        case EVENT_TYPE_CONNECTION:
            break;
        }
    }

//    while(true) {
//        connection = server_tcp_accept(listen);
//        if (listen->process) {
//            listen->process(connection);
//        }
//    }
}

int epoll_add_event(int efd, int fd, struct epoll_event *event) {
    struct epoll_event   ee;

    ee.data.ptr = event->data.ptr;
    ee.events   = EPOLLIN | EPOLLOUT | event->events;

    if (success != epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ee)) {
        printf("epoll_ctl failed!(%s)\n", strerror(errno));
        exit(1);
    }

    return success;
}

int epoll_del_event(int efd, int fd, struct epoll_event *event) {
    struct epoll_event   ee;

    ee.data.ptr = event->data.ptr;
    ee.events  = EPOLLIN | EPOLLOUT | event->events;

    if (success != epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ee)) {
        printf("epoll_ctl failed!(%s)\n", strerror(errno));
        exit(1);
    }

    return success;
}
