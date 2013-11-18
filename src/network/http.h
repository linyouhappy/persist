#ifndef H_NETWORK_HTTP_INCLUDED
#define H_NETWORK_HTTP_INCLUDED

#include "network/server.h"

int http_get_request_header(string_p dst, u_char * src);
int http_get_request_body(string_p dst, u_char *src);

#endif
