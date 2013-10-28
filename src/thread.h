#ifndef H_THREAD_INCLUDED
#define H_THREAD_INCLUDED

#include "persist.h"
#include <pthread.h>

typedef struct thread_pool_s        thread_pool_t;
typedef thread_pool_t *             thread_pool_p;

typedef struct thread_worker_s      thread_worker_t;
typedef thread_worker_t *           thread_worker_p;

typedef struct thread_task_s        thread_task_t;
typedef thread_task_t *             thread_task_p;

typedef void *(*thread_worker_process)(void *, thread_worker_p);

//  默认线程数量
#define thread_pool_count        5
#define thread_pool_min          2
#define thread_pool_max          10;

struct thread_pool_s {
    int                            count;               //  线程数量
    int                            min;                 //  最少数量
    int                            max;                 //  最多数量
    int                            close;               //  关闭状态
    thread_worker_p                workers;             //  线程列表
    thread_task_p                  task;
    pthread_cond_t                *cond;                //  条件
    pthread_mutex_t               *mutex;               //  互拆锁

    int                            task_last_id;        //  任务自增id
};

struct thread_worker_s {
    int                            id;
    pthread_t                      thread;
    int                            status;
    thread_pool_p                  pool;
    thread_task_p                  task;
};

struct thread_task_s {
    int                             id;
    thread_worker_process           process;
    void *                          args;

    thread_task_p                   prev;
    thread_task_p                   next;
};

thread_pool_p thread_pool_init();
thread_pool_p thread_pool_create(thread_pool_p /*, thread_worker_process */);
int thread_pool_destroy(thread_pool_p);

thread_task_p thread_task_init();

int thread_task_add(thread_pool_t * pool, thread_worker_process process, void * args);

#endif
