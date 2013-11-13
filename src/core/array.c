#include "core/array.h"

int array_init(array_p array, int n, size_t size) {
    array->nelts = 0;
    array->size = size;
    array->max  = n;

    array->elts = malloc(n * size);
    if (array->elts == NULL) {
        return failed;
    }

    return success;
}

array_p array_create(int n, size_t size) {
    array_t *a;

    a = malloc(sizeof(array_t));
    if (a == NULL) {
        return NULL;
    }

    if (array_init(a, n, size) != success) {
        return NULL;
    }

    return a;
}


void array_destroy(array_t *a) {
    free(a->elts);
    free(a);
}

void * array_push(array_t *a) {
    void        *elt, *new;
    size_t       size;

    if (a->nelts == a->max) {
        size = a->size * a->max;

        new = malloc(2 * size);
        if (new == NULL) {
            return NULL;
        }

        free(a->elts);

        memcpy(new, a->elts, size);
        a->elts = new;
        a->max *= 2;
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}


void * array_npush(array_t *a, int n) {
    void        *elt, *new;
    size_t       size;
    int           nalloc;

    size = n * a->size;

    if (a->nelts + n > a->max) {

        nalloc = 2 * ((n >= a->max) ? n : a->max);

        new = malloc(nalloc * a->size);
        if (new == NULL) {
            return NULL;
        }

        memcpy(new, a->elts, a->nelts * a->size);
        a->elts   = new;
        a->max = nalloc;
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    return elt;
}
