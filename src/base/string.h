#ifndef H_BSTRING_INCLUDED
#define H_BSTRING_INCLUDED

#include "persist.h"

typedef struct string_s             string_t;
typedef string_t *                  string_p;

#define memzero(buf, n)       (void) memset(buf, 0, n)

struct string_s {
    size_t      len;
    u_char     *data;
};


typedef struct {
    string_t   key;
    string_t   value;
} keyval_t;

#define string(str)     { sizeof(str) - 1, (u_char *) str }
#define string_null     { 0, NULL }

#define str_set(str, text)                          \
    (str)->len = sizeof(text) - 1;                  \
    (str)->data = (u_char *) text
#define str_null(str)                               \
    (str)->len = 0;                                 \
    (str)->data = NULL


#define tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

#endif
