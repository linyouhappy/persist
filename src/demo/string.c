#include "demo/string.h"

void string_demo_low(void) {
    printf("string_demo_low\n");
}

void string_demo_strstr(void) {
    string_t string = string("Sec-WebSocket-Key: WKXWzEn7/f0ohyzl7fAPTg==");
    string_t search = string("Sec-WebSocket-Key: ");
    u_char *ptr;
//
    ptr = mstrstr(SVAL(string), SVAL(search));

    printf("%s\n", ptr);
}

void string_demo_base64(void) {
    string_t src = string("12345678");
    string_t des;

    des.data = malloc(sizeof(src.data));

    printf("(%02ld>", base64_encoded_length(SLEN(src)));
    base64_encode(&des, &src);
    printf("%02ld):%s\n", SLEN(des), SVAL(des));

    printf("(%02ld>", base64_decoded_length(SLEN(des)));
    base64_decode(&des, &des);
    printf("%02ld):%s\n", SLEN(des), SVAL(des));
}
