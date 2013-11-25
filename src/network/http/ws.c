#include "network/http/ws.h"

string_t http_ws_key = string("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
string_t http_ws_request[] = {
    string("Host"),
    string("Origin"),
    string("Sec-WebSocket-Key"),
    string("Sec-WebSocket-Version"),
    string("Sec-WebSocket-Protocol"),
    string("Sec-WebSocket-Extensions"),
    string_null,
};

keyval_t http_ws_response[] = {
    keyval("HTTP/1.1 101 Switching Protocols", null),        //  HEADER
    keyval("Upgrade:", "websocket"),
    keyval("Connection:", "Upgrade"),
//    keyval("Server:", "Persist Server"),    //  SERVER
    keyval("Sec-WebSocket-Accept:", null),
//    keyval("Sec-WebSocket-Protocol:", "chat"),
    keyval_null,
};

string_p http_ws_k2a(u_char * key, size_t len) {
    string_t sha;
    string_p ret;
    u_char   *p;

    ret = mmalloc(sizeof(string_t));

//    printf("key(%ld):%s\n", len, key);

    p = SHA1(key, len, null);

    //  OLD
//    sha.len = mstrlen(p);
//    sha.data = p;
    //  NEW
    sha.len = 20;   // @TODO  mstrlen(p) 有时候P长度不是20  SHA1输入为60位，人工写死
    sha.data = p;

//    printf("sha(%ld):%s\n", sha.len, sha.data);

    ret->len  = base64_encoded_length(sha.len);
    ret->data = mmalloc(ret->len);

    base64_encode(ret, &sha);
//    printf("ret(%ld):%s\n", ret->len, ret->data);

    //@TODO  p 是否需要销毁
//    free(p);
    return ret;
}

uint http_ws_parse(http_ws_frame_p frame, u_char * buff) {
    uint     i, offset;
    u_char * p, mask[4];

    p = buff;

    //  1 BYTE
    frame->fin  = (*p & 0x80) >> 7;
    frame->rsv1 = 0;
    frame->rsv2 = 0;
    frame->rsv3 = 0;
    frame->opcode = (*p & 0x0F);
    //  2 BYTE
    frame->mask         = (*(p+1) & 0x80)>>7;
    frame->payload_len  = *(p+1) & 0x7F;

    if (frame->mask) {
        mask[0] = *(p+2);
        mask[1] = *(p+3);
        mask[2] = *(p+4);
        mask[3] = *(p+5);
    }

    offset = 6;
    for(i=0; i<frame->payload_len; i++) {
        buff[offset] = *(p+offset) ^ mask[(i%4)];
        offset++;
    }

    frame->payload_data = buff+offset-frame->payload_len;

#if 0
    printf("=======\n");
    printf("fin:%x\n", frame->fin);
    printf("payload_len:%d\n", frame->payload_len);
    printf("opcode:%x\n", frame->opcode);
    printf("=======\n");
#endif
    return success;
}

