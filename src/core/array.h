#ifndef H_BARRAY_INCLUDED
#define H_BARRAY_INCLUDED

#include "persist.h"

typedef struct array_s              array_t;
typedef array_t *                   array_p;

struct array_s {
    void        *elts;
    int          nelts;                 //  元素个数
    size_t       size;
    int          max;                   //  最大个数
};

int array_init(array_t *array, int n, size_t size);
array_t *array_create(int n, size_t size);
void array_destroy(array_t *a);
void *array_push(array_t *a);
void *array_push_n(array_t *a, int n);

#endif
