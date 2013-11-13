#include "persist.h"

int main(const int argc, const char ** argv) {
//    hash_demo_test();
    u_char str[0];


    u_char text[5] = "aaaa";


    strlow(str, text, 5);

    printf("%s\n", str);

    printf("%c\n", *str);
    printf("%c\n", *str+1);
    printf("%c\n", *str+2);
    printf("%c\n", *str+3);
    return 0;
}
