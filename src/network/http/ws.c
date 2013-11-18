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

string_p http_ws_k2k(u_char * key, size_t len) {
    string_t sha;
    string_p ret;
    u_char   *p;

    ret = mmalloc(sizeof(string_t));

    p = SHA1(key, len, null);

    sha.len = mstrlen(p);
    sha.data = p;

    ret->len  = base64_encoded_length(sha.len);
    ret->data = mmalloc(ret->len);

    base64_encode(ret, &sha);

    //@TODO  p 是否需要销毁
//    free(p);
    return ret;
}

