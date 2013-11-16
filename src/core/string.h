#ifndef H_BSTRING_INCLUDED
#define H_BSTRING_INCLUDED

#include "core/core.h"

typedef struct string_s                          string_t;
typedef string_t *                               string_p;

typedef struct keyval_s                          keyval_t;
typedef keyval_t *                               keyval_p;

#define SVAL_P(str)                              (str)->data
#define SVAL(str)                                SVAL_P(&str)

#define SLEN_P(str)                              (str)->len
#define SLEN(str)                                SLEN_P(&str)

#define string(str)                              { sizeof(str) - 1, (u_char *) str }
#define string_null                              { 0, NULL }

#define keyval(key, value)                       { string(key), string(value) }
#define keyval_null                              { string_null, string_null }
#define memzero(buf, n)                   (void) memset(buf, 0, n)

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

//  字符大小写转换
#define strclow(c)                              (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define strcupp(c)                              (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

//  字符串对比
#define mstrcmp(s1, s2)                         strcmp((const char *) s1, (const char *) s2)
#define mstrncmp(s1, s2, n)                     strncmp((const char *) s1, (const char *) s2, n)
//  字符串搜索
#define mstrstr(s1, s2)                         (u_char *) strstr((const char *) s1, (const char *) s2)
#define mstrnstr(s1, s2, n)                     (u_char *) strnstr(const char *s1, const char *s2, size_t n);

//  字符串长度
#define mstrlen(s)                              strlen((const char *) s)
//  字符搜索
#define mstrchr(s1, c)                          (u_char *) strchr((const char *) s1, (int) c)

//  字节对齐
#define align(d, a)                             (((d) + (a - 1)) & ~(a - 1))
#define align_ptr(p, a)                         (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

//  字符串大小写转换
void strnlow(u_char *dst, u_char *src, size_t n);
void strnupp(u_char *dst, u_char *src, size_t n);

//  @TODO 补strnstr


/*
 * BASE64函数
 * @NOTE 这两个方法是估算长度，不一定精准，但是可以做到分配内存，多几个字节无所谓
 */
#define base64_encoded_length(len)   (((len + 2) / 3) * 4)
#define base64_decoded_length(len)  (((len + 3) / 4) * 3)

void base64_encode(string_p dst, string_p src);
uint base64_decode(string_p dst, string_p src);
uint base64_url_decode(string_p dst, string_p src);

//  内存
void * mmalloc(size_t size);
//  内存拷贝
#define mmemcpy(dst, src, n)                    (void) memcpy(dst, src, n)
#define mcpymem(dst, src, n)                    (((u_char *) memcpy(dst, src, n)) + (n))

//  内存移动
#define mmemmove(dst, src, n)                  (void) memmove(dst, src, n)
#define mmovemem(dst, src, n)                 (((u_char *) memmove(dst, src, n)) + (n))

//  内存对比
#define mmemcmp(s1, s2, n)                      memcmp((const char *) s1, (const char *) s2, n)
#endif
