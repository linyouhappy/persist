#include "demo/hash.h"

void hash_demo_test(void) {
    hash_init_t  hinit;
    array_t      array;
    int          i, c;

    hash_key_p   note;

    keyval_t     list[] = {
        keyval("a", "1"),
        keyval("b", "2"),
    };


    array_init(&array, 10, sizeof(hash_key_t));

    c = array_count(list, keyval_t);

    for(i=0; i<c; i++) {
        note = (hash_key_t*) array_push(&array);

        note->key   = list[i].key;
        note->hash  = hash_key_lc(list[i].key.data, list[i].key.len);
        note->value = list[i].value.data;
        printf("key: %s ,\tkey_hash: %u\n", note->key.data, note->hash);
    }


    hinit.hash        = malloc(sizeof(hash_t));
    hinit.name        = "hahs_test";
    hinit.key         = &hash_key_lc;
    hinit.max_size    = 8;
    hinit.bucket_size = 128;

    hash_init(&hinit, array.elts, array.nelts);

    printf("success\n");

}
