#include "persist.h"

//static void * my_process(void * args, thread_worker_p worker) {
//    printf("worder(%d):task(%d)\n", worker->id, worker->task->id);
//    sleep(2);
//    return worker;
//}
//
//// thread_pool_demo
//static void thread_pool_demo() {
//    thread_pool_p pool;
//    pool = thread_pool_create(null);
//    thread_task_add(pool, my_process, NULL);
//    thread_pool_destroy(pool);
//}


void server_tcp_accept(event_p ev) {
//connection_p server_tcp_accept(listening_p listen) {
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
    c->read  = malloc(sizeof(event_t));
    c->write = malloc(sizeof(event_t));

    read = c->read;
    read->data    = c;
    read->process = server_read;

    printf("%d\n", c->fd);

    ret = event_actions.add(read, EVFILT_READ, 0);

    if (-1 == ret) {
        printf("kqueue_add_event failed ! (%s)\n", strerror(errno));
        exit(1);
    }
}

void server_tcp_read(event_p ev) {
    connection_p c;
    c = (connection_p) ev->data;
    close(c->fd);
    printf("client:%d outing\n", c->fd);
}

void server_tcp_write(event_p ev) {
    printf("%s\n", "write");
}

int main(void) {
    server_t server;
    server.port = 16622;
    server.ip   = null;
    server.accept = server_tcp_accept;
    server.read   = server_tcp_read;
    server.write  = server_tcp_write;

    server_tcp_create(&server);
    server_tcp_process();

    return EXIT_SUCCESS;
}
