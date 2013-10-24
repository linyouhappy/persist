#include "thread.h"

static void * thread_pool_worker(void * args) {
    thread_worker_p     worker;
    worker = args;

    if (NULL == worker->process) {
        printf("%d:no action\n", worker->id);
    } else {
        worker->process((void *) worker);
    }

    return (void *)worker;
}

thread_pool_p thread_pool_init() {
    thread_pool_p   p;
    p = (thread_pool_p) malloc(sizeof(thread_pool_t));
    return p;
}

thread_pool_p thread_pool_create(thread_pool_p pool, thread_worker_process process) {
    int                 i;
    thread_worker_p     worker;

    if (null == pool) {
        thread_pool_p       pool;
        pool           = thread_pool_init();
        pool->num      = thread_pool_num;
    }

    pool->workers  = (thread_worker_p) malloc(pool->num * sizeof(thread_worker_t));

    for(i=0; i<pool->num; i++) {
        worker = pool->workers + i;
        worker->id = i;
        worker->process = process;
        worker->pool    = pool;

        pthread_create(&worker->thread, NULL, &thread_pool_worker, worker);
    }

    return pool;
}

int thread_pool_destroy(thread_pool_p pool) {
    int   i, r;

    for(i=0; i<pool->num; i++) {
        r = pthread_join((pool->workers + i)->thread, NULL);
        if (0 != r) {
            perror("pthread_join:");
            exit(0);
        }
    }
    free(pool->workers);
    free(pool);
    printf("success\n");
    return EXIT_SUCCESS;
}
