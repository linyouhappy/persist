#include "persist.h"

static void * my_process(void * args) {
    thread_worker_p worker;
    worker = args;
    printf("%d:action\n", worker->id);
    return worker;
}

int main(void) {
    thread_pool_p pool;
    pool = malloc(sizeof(thread_pool_t));
    pool->num = 10;

    pool = thread_pool_create(pool, my_process);

    thread_pool_destroy(pool);

    return EXIT_SUCCESS;
}
