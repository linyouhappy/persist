#include "string.h"

void strlow(u_char *dst, u_char *src, size_t n) {
    while (n) {
        *dst = tolower(*src);
        dst++;
        src++;
        n--;
    }
}
