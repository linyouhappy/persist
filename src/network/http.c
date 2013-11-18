#include "network/http.h"

int http_get_request_header(string_p dst, u_char * src) {
    u_char *p, *end;
    p   = src;
    end = mstrstr(p, "\r\n\r\n");

    dst->data = src;
    dst->len  = end-p;

#if 0
    u_char * header;
    header = mmalloc(dst->len+1);
    mmemcpy(header, dst->data, dst->len);
    printf("header(%ld):%s\n", dst->len, header);
#endif

    return success;
}

int http_get_request_body(string_p dst, u_char * src) {
    u_char *p, *end;
    p   = src;
    end = mstrstr(p, "\r\n\r\n");
    dst->data = end+4;

    end = mstrrchr(src, '\0');
    dst->len  = end - dst->data;

    if (0 == dst->len) {
        dst->data = null;
    }
#if 0
    u_char * body;
    body = mmalloc(dst->len+1);
    mmemcpy(body, dst->data, dst->len);
    printf("body(%ld):%s\n", dst->len, body);
#endif
    return success;
}
