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

//void _read(connection_p c) {
//    printf("%d:read\n", c->fd);
//}
//
//void _close(connection_p c) {
//    printf("%d:close\n", c->fd);
//}

int main(void) {
    //  server example
//    server_p server = server_initialize();
//    server->port = 16622;
//    server->ip   = null;
//
//    server->read  = _read;
//    server->close = _close;
//
//    server_tcp_create(server);
//    server_tcp_process();


    //  array example
    int i;
    array_p  array;
    string_p string;
    array = array_create(10, sizeof(string_t));

    if (null == array) {
        printf("array_create failed!");
    }

    string = array_push(array);
    str_set(string, "aaa111");

    string = array_push(array);
    str_set(string, "bbb222");

    for(i=0; i<array->nelts; i++) {
        string = array->elts + i * array->size;

        printf("%s\n", string->data);
    }
    printf("success\n");

    return EXIT_SUCCESS;
}
