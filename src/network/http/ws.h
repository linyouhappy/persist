#ifndef H_NETWORK_HTTP_WS_INCLUDED
#define H_NETWORK_HTTP_WS_INCLUDED

typedef struct http_ws_frame_s          http_ws_frame_t;
typedef http_ws_frame_t *               http_ws_frame_p;

#include "network/http.h"
#include <openssl/sha.h>

#define NTWS_OPCODE_TEXT                1   //  文本
#define NTWS_OPCODE_CLOSE               8   //  关闭

struct http_ws_frame_s {
    //  1 BYTE
    uint        fin:1;
    uint        rsv1:1;
    uint        rsv2:1;
    uint        rsv3:1;
    uint        opcode:4;
    //  2 BYTE
    uint        mask:1;
    uint        payload_len:7;

    uint        extended_payload_length;
    u_char      *payload_data;
};

string_p http_ws_k2a(u_char *, size_t);

uint http_ws_parse(http_ws_frame_p, u_char *);

#endif
