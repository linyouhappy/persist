#include "network/server.h"

server_p server_init() {
    server_p server;
    server = (server_p) mmalloc(sizeof(server_t));
    return server;
}

//  创建服务器
int server_tcp_create(server_p server) {
    listening_p  listening;
    connection_p connection;
    event_p      read /*, write */;
    int          ret;
    struct sockaddr_in * sockaddr;

    listening = (listening_p) mmalloc(sizeof(listening_t));

//    listening->process = null;

    listening->socklen = sizeof(struct sockaddr);
    sockaddr = (struct sockaddr_in *) &listening->sockaddr;

    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port   = htons(server->port);
    if (null == server->ip) {
        sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    }

    listening->fd = socket(AF_INET, SOCK_STREAM, 0);

    // @todo 复用
    int flag=1,len=sizeof(int);
    if(-1 == setsockopt(listening->fd, SOL_SOCKET, SO_REUSEADDR, &flag, len)) {
        perror("setsockopt");
        exit(1);
     }

    if (-1 == bind(listening->fd, (struct sockaddr *)sockaddr, sizeof(struct sockaddr))) {
        printf("bind failed!(%s)\n", strerror(errno));
        exit(0);
    }

    if (-1 == listen(listening->fd, 10)) {
        printf("listen failed!\n");
    }

    server_accept = server_tcp_accept;
    server_read   = server_tcp_read;
    server_write  = server_tcp_write;

    server_user_accept = server->accept;
    server_user_read   = server->read;
    server_user_close  = server->close;

    //  事件初始化
    event_init();

    connection = (connection_p) mmalloc(sizeof(connection_t));
    connection->fd = listening->fd;
    connection->listening = listening;
    listening->connection = connection;

    connection->read  = mmalloc(sizeof(event_t));
    connection->write = mmalloc(sizeof(event_t));

    read = connection->read;
    read->data    = connection;
    read->process = server_accept;


    ret = event_actions.add(read, EVFILT_READ, 0);

    if (-1 == ret) {
        printf("kqueue_add_event failed ! (%s)\n", strerror(errno));
        exit(1);
    }
    return success;
}

int server_tcp_process() {
    //kqueue
    int ret;

    ret = kqueue_process_events();
    if(-1 == ret) {
        printf("error\n");
    }

    return success;
}

void server_tcp_accept(event_p ev) {

    listening_p          ls;
    connection_p         lc, c;
    event_p              read;
    int                  ret;

    lc = ev->data;
    ls = lc->listening;

    c = (connection_p) malloc(sizeof(connection_t));
    memset(c, 0, sizeof(connection_t));

    c->socklen  = sizeof(struct sockaddr_in);

    c->fd = accept(ls->fd, (struct sockaddr *)&c->sockaddr, &c->socklen);

    //  设置connection
    c->listening = ls;
    c->read  = mmalloc(sizeof(event_t));
    c->write = mmalloc(sizeof(event_t));

    read = c->read;
    read->data    = c;

    read->process = server_read;

    ret = event_actions.add(read, EVFILT_READ, 0);

    if (-1 == ret) {
        printf("kqueue_add_event failed ! (%s)\n", strerror(errno));
        exit(1);
    }

    if (null != server_user_accept) {
        server_user_accept(c);
    } else {
        printf("没有SERVER_USER_ACCEPT\n");
    }
}

void server_tcp_read(event_p ev) {
    connection_p c;
    int          n;
    c = (connection_p) ev->data;

    if (ev->size == 0) {
        if (-1 == close(c->fd)) {
            printf("close failed\n");
        }

        if(null != server_user_close) {
            server_user_close(c);
        } else {
            printf("没有SERVER_USER_CLOSE\n");
        }

    } else {
        if (null != server_user_read) {
            server_user_read(ev);
        } else {
            printf("没有SERVER_USER_READ\n");
        }
    }
}

void server_tcp_write(event_p ev) {
    printf("%s\n", "write");
}
