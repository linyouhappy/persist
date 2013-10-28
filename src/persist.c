#include "persist.h"

static void * my_process(void * args, thread_worker_p worker) {
    printf("worder(%d):task(%d)\n", worker->id, worker->task->id);
    sleep(2);
    return worker;
}

int main(void) {
    thread_pool_p pool;
    pool = thread_pool_create(null);
    thread_task_add(pool, my_process, NULL);
    thread_pool_destroy(pool);
    return EXIT_SUCCESS;
}
