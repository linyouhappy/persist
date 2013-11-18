#include "demo/server.h"
#include <openssl/sha.h>

extern string_t http_ws_key;
extern keyval_t http_ws_response[];

//static keyval_t TEXT_HEADER[] = {
//    keyval("HTTP/1.1 200 OK", null),        //  HEADER
//    keyval("Server:", "Persist Server"),    //  SERVER
//    keyval("Content-Length: ", null),
//    keyval("Content-Type: text/html;charset=UTF-8", null),  //  Content-Type
//    keyval_null,
//};
//
//static string_t TEXT_BODY = string("text content");

static void server_demo_accept(connection_p conn) {
    printf("[user]%d:accept\n", conn->fd);
}

static void server_demo_read(event_p ev) {
    int  fd;
    uint n;
    u_char buff[1024];
    connection_p conn;

    conn = ev->data;
    fd   = conn->fd;

    memzero(buff, 1024);

    if (-1 == (n = read(fd, buff, 1024))) {
        printf("read failed!(%s)\n", strerror(errno));
        exit(1);
    }

    printf("(read)%s\n", buff);
}

static void server_demo_auth(event_p ev) {
    int  fd;
    uint n, size;
    connection_p conn;
    string_t header, body;

    conn = ev->data;
    size = ev->size;
    fd   = conn->fd;

    u_char buff[1024];
    memzero(buff, 1024);

    if (-1 == (n = read(fd, buff, 1024))) {
        printf("read failed!(%s)\n", strerror(errno));
        exit(1);
    }

    http_get_request_header(&header, buff);
//  获取KEY
    u_char  *end;
    string_t key;
//    uint     len;
    key.data = mstrstr(header.data, "Sec-WebSocket-Key")+sizeof("Sec-WebSocket-Key")-1+2;
//    memcpy(key.data, "dGhlIHNhbXBsZSBub25jZQ==", sizeof("dGhlIHNhbXBsZSBub25jZQ==")-1);

    end = mstrstr(key.data, "\r\n");
    key.len  = end - (key.data);
//    printf("(%ld)%s\n", key.len, key.data);
//    printf("%ld\n", key.len);

    string_t skey;
    string_p akey;
    u_char *p;

    skey.len  = key.len+http_ws_key.len;
    skey.data = mmalloc(skey.len);

    p = mcpymem(skey.data, key.data, key.len);
    p = mcpymem(p, http_ws_key.data, http_ws_key.len);

//    printf("(%ld)%s\n", skey.len, skey.data);

    akey = http_ws_k2k(skey.data, skey.len);

/// #############
    http_get_request_body(&body, buff);
/// ###########################################################################################
//    exit(1);

    //  response
//    size_t   c;
    int      i;
//    char     bc[8];
//    bc = SLEN(TEXT_BODY);
//    c = array_count(http_ws_response, keyval_t);

    for(i=0; i<4; i++) {
        if (SVAL(http_ws_response[i].key) == null) {
            continue;
        }
        write(fd, SVAL(http_ws_response[i].key), SLEN(http_ws_response[i].key));
        printf("%s", SVAL(http_ws_response[i].key));

        //  写入ACCEPT KEY
        switch(SLEN(http_ws_response[i].key)) {
        case 21:
            if (0 == mstrncmp(SVAL(http_ws_response[i].key), "Sec-WebSocket-Accept:", 21)) {
                write(fd, akey->data, akey->len);
                printf("%s", akey->data);
            }
            break;
        }
        if (SVAL(http_ws_response[i].value)) {
            write(fd, SVAL(http_ws_response[i].value), SLEN(http_ws_response[i].value));
            printf("%s", SVAL(http_ws_response[i].value));
        }
        printf("\n");
        write(fd, "\r\n", 2);
    }
    write(fd, "\r\n", 2);
    write(fd, "\n", 1);

    ev->process = server_demo_read;

//  response end

}

static void server_demo_close(connection_p conn) {
    printf("[user]%d:close\n", conn->fd);
}

void server_demo_example(void) {

    server_p server;

    server = server_init();

    server->port = 57717;

    server->accept = server_demo_accept;
    server->read  = server_demo_auth;
    server->close = server_demo_close;

    if (failed == server_tcp_create(server)) {
        printf("create server failed!\n");
        exit(1);
    }
    printf("server(%s:%d) create success!\n", (null == server->ip) ? "127.0.0.1" : server->ip, server->port);

    server_tcp_process();

}
