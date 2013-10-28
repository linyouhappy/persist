#include "persist.h"

static void * my_process(void * args, thread_worker_p worker) {
    printf("worder(%d):task(%d)\n", worker->id, worker->task->id);
    sleep(2);
    return worker;
}

int main(void) {
    thread_pool_p pool;
    pool = (thread_pool_p)malloc(sizeof(thread_pool_t));
    pool->num = 2;

    pool = thread_pool_create(pool);
    thread_task_add(pool, my_process, NULL);
    sleep(5);
    thread_pool_destroy(pool);
    return EXIT_SUCCESS;
}
