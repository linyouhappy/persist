#include "thread.h"

static void * thread_pool_worker(void * args) {
    thread_worker_p worker = args;
    thread_pool_p   pool   = worker->pool;
    thread_task_p   task;

    if (!pthread_equal(worker->thread, pthread_self())) {
        printf("不相等!\n");
    }

    while(true) {
        pthread_mutex_lock(pool->mutex);
        if ((pool->close == false) && queue_empty(pool->task)) {
#if 1
            printf("worker(%d): 进入等待\n", worker->id);
#endif
            pthread_cond_wait(pool->cond, pool->mutex);
        }

        if (pool->close == true) {
#if 1
            printf("worker(%d): 关闭\n", worker->id);
#endif
            pthread_mutex_unlock(pool->mutex);  //  解锁
            return NULL;
        }

        //  队列里有任务  处理任务
        if (!queue_empty(queue_head(pool->task))) {
            //  弹出一个任务
            task = queue_head(pool->task);
            queue_remove(task);
            pthread_mutex_unlock(pool->mutex);

//            if (NULL != task->name) {
//                prctl_set_name(task->name);
//            }
//
//            worker_args = malloc(sizeof(thread_pool_worker_info_t));
//
//            worker_args->wid  = self->id;  //  worker  id
//            worker_args->tid  = task->id;  //  thread  id
//
            if (task->process) {  //  如果函数复制，执行函数
                worker->task = task;
                task->process(task->args, worker);
                free(task);
            }
        } else {
            pthread_mutex_unlock(pool->mutex);
        }
    }

//    if (NULL == worker->process) {
//        printf("%d:no action\n", worker->id);
//    } else {
//        worker->process((void *) worker);
//    }

    return (void *)worker;
}

thread_pool_p thread_pool_init() {
    thread_pool_p   p;
    p = (thread_pool_p) malloc(sizeof(thread_pool_t));
    return p;
}

thread_pool_p thread_pool_create(thread_pool_p pool) {
    int                 i;
    thread_worker_p     worker;

    if (null == pool) {
        thread_pool_p       pool;
        pool           = thread_pool_init();
        pool->num      = thread_pool_num;
    }
    pool->close    = false;
    pool->task     = (thread_task_p) malloc(sizeof(thread_task_t));
    queue_init(pool->task);

    pool->cond     = (pthread_cond_t *)  malloc(sizeof(pthread_cond_t));
    pool->mutex    = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    pool->task_last_id  = 0;

    if (pthread_cond_init(pool->cond, NULL) != 0) {
        printf("%s: pthread_cond_init failed, errno:%d, error:%s\n",
                __FUNCTION__, errno, strerror(errno));
        exit(1);
    }

    if (pthread_mutex_init(pool->mutex, NULL) != 0) {
        printf("%s: pthread_mutex_init failed, errno:%d, error:%s\n",
                __FUNCTION__, errno, strerror(errno));
        exit(1);
    }

    pool->workers  = (thread_worker_p) malloc(pool->num * sizeof(thread_worker_t));

    for(i=0; i<pool->num; i++) {
        worker = pool->workers + i;
        worker->id = i;
        worker->pool    = pool;
        worker->task    = null;

        pthread_create(&worker->thread, NULL, &thread_pool_worker, worker);
    }

    return pool;
}

int thread_pool_destroy(thread_pool_p pool) {
    int   i, r;

    if (pool->close) {  //  如果线程吃已经关闭  直接返回0
        return 0;
    }

    pool->close = true;

    pthread_mutex_lock(pool->mutex);
    pthread_cond_broadcast(pool->cond);  //  广播信号
    pthread_mutex_unlock(pool->mutex);

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

int thread_task_add(thread_pool_t * pool, thread_worker_process process, void * args) {
    thread_task_p task;

    task = malloc(sizeof(thread_task_t));
    task->process = process;
    task->args    = args;
    task->id      = pool->task_last_id++;

    pthread_mutex_lock(pool->mutex);      //  枷锁
    queue_insert_tail(pool->task, task);  //  把任务插入到尾部
    pthread_cond_signal(pool->cond);      //  广播信号
    pthread_mutex_unlock(pool->mutex);    //  解锁

#if 1
    printf("添加任务: 任务号(%u)\n", task->id);
#endif
    return task->id;
}
