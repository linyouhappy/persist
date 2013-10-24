#ifndef H_THREAD_INCLUDED
#define H_THREAD_INCLUDED

// TODO : 1 thread pool
#include "persist.h"
#include <pthread.h>

typedef struct thread_pool_s        thread_pool_t;
typedef thread_pool_t *             thread_pool_p;

typedef struct thread_worker_s      thread_worker_t;
typedef thread_worker_t *           thread_worker_p;

typedef void *(*thread_worker_process)(void *);

//  默认线程数量
#define thread_pool_num        5

struct thread_pool_s {
    int                     num;        //  线程数量
    thread_worker_p         workers;    //  线程列表
};

struct thread_worker_s {
    int                            id;
    pthread_t                      thread;
    thread_worker_process          process;
    int                            status;
    thread_pool_p                  pool;
};

int thread_pool_destroy(thread_pool_p);
thread_pool_p thread_pool_init();
thread_pool_p thread_pool_create(thread_pool_p, thread_worker_process);

#endif
