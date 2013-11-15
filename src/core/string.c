#include "core/core.h"

void strlow(u_char *dst, u_char *src, size_t n) {
    while (n) {
        *dst = tolower(*src);
        dst++;
        src++;
        n--;
    }
}

//  内存
void * mmalloc(size_t size) {
    void * p;
    p = malloc(size);
    memzero(p, size);
    return p;
}
