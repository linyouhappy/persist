#ifndef H_BSTRING_INCLUDED
#define H_BSTRING_INCLUDED

#include "core/core.h"

typedef struct string_s             string_t;
typedef string_t *                  string_p;

typedef struct keyval_s             keyval_t;
typedef keyval_t *                  keyval_p;

#define strval_p(str)               str->data;
#define sv(str)                     strval_p(&str);

#define strlen_p(str)               str->len;
#define sl(str)                     strlen_p(&str);

#define string(str)                 { sizeof(str) - 1, (u_char *) str }
#define string_null                 { 0, NULL }

#define keyval(key, value)          { string(key), string(value) }
#define keyval_null                 { string_null, string_null }
#define memzero(buf, n)       (void) memset(buf, 0, n)

extern int  cacheline;

struct string_s {
    size_t      len;
    u_char     *data;
};

struct keyval_s {
    string_t   key;
    string_t   value;
};

#define str_set(str, text)                          \
    (str)->len = sizeof(text) - 1;                  \
    (str)->data = (u_char *) text
#define str_null(str)                               \
    (str)->len = 0;                                 \
    (str)->data = NULL


#define tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

//  字节对齐
#define align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

void strlow(u_char *dst, u_char *src, size_t n);

//  内存
void * mmalloc(size_t size);
#endif
