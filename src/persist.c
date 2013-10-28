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

int main(void) {
    listening_p  listen;
    connection_p connection;
    listen = server_tcp_create(16622, null);

    connection = server_tcp_accept(listen);

    printf("success\n");

    return EXIT_SUCCESS;
}
