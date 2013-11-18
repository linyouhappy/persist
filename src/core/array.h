#ifndef H_CORE_ARRAY_INCLUDED
#define H_CORE_ARRAY_INCLUDED

#include "core/core.h"

#define array_count(var, struct)      sizeof(var)/sizeof(struct)

typedef struct array_s              array_t;
typedef array_t *                   array_p;

struct array_s {
    void        *elts;
    int          nelts;                 //  元素个数
    size_t       size;
    int          max;                   //  最大个数
};

int array_init(array_p array, int n, size_t size);
array_p array_create(int n, size_t size);
void array_destroy(array_p a);
void *array_push(array_p a);
void *array_push_n(array_p a, int n);

#endif
