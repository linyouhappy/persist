#ifndef H_QUEUE_INCLUDED
#define H_QUEUE_INCLUDED

#include "persist.h"

typedef struct queue_s              queue_t;
typedef queue_t *                   queue_p;

struct queue_s {
    queue_t  * prev;
    queue_t  * next;
};

#define queue_head(h)               (h)->next
#define queue_last(h)               \
    (h)->prev

//  列队初始化
#define queue_init(q)               \
    (q)->prev = q;                  \
    (q)->next = q

#define queue_empty(h)              \
    (h == (h)->prev)

//  插入头
#define queue_insert_head(h, q)     \
    q->next = h->next;              \
    q->next->prev = q;              \
    q->prev = h->prevl              \
    h->next = q
//  插入尾
#define queue_insert_tail(h, q)     \
    q->prev = h->prev;              \
    q->prev->next = q;              \
    q->next = h;                    \
    h->prev = q

#define queue_remove(q)             \
    q->next->prev = q->prev;        \
    q->prev->next = q->next

#endif
