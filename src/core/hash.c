#include "persist.h"

int hash_key(u_char *data, size_t len) {
    int  i, key;

    key = 0;

    for (i = 0; i < len; i++) {
        key = hash(key, data[i]);
    }

    return key;
}


int hash_key_lc(u_char *data, size_t len) {
    int  i, key;

    key = 0;

    for (i = 0; i < len; i++) {
        key = hash(key, tolower(data[i]));
    }

    return key;
}

void * hash_find(hash_t *hash, int key, u_char *name, size_t len) {
    int       i;
    hash_elt_t  *elt;

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[key % hash->size];

    if (elt == NULL) {
        return NULL;
    }

    while (elt->value) {
        if (len != (size_t) elt->len) {
            goto next;
        }

        for (i = 0; i < len; i++) {
            if (name[i] != elt->name[i]) {
                goto next;
            }
        }

        return elt->value;

    next:

        elt = (hash_elt_t *) align_ptr(&elt->name[0] + elt->len,
                                               sizeof(void *));
        continue;
    }

    return NULL;
}

int hash_init(hash_init_p hinit, hash_key_p names, int nelts) {
    //@TODO
    cpuinfo();

    u_char          *elts;
    size_t           len;
    u_short         *test;
    unsigned long    i, n, key, size, start, bucket_size;
    hash_elt_t  *elt, **buckets;

    for (n = 0; n < nelts; n++) {
        if (hinit->bucket_size < HASH_ELT_SIZE(&names[n]) + sizeof(void *)) {
            printf("could not build the %s, you should "
                   "increase %s_bucket_size: %i",
                    hinit->name, hinit->name, hinit->bucket_size);
            return failed;
        }
    }

    test = malloc(hinit->max_size * sizeof(u_short));
#if 1
    printf("test malloc size:%ld\n", hinit->max_size * sizeof(u_short));
#endif
    if (test == NULL) {
        return failed;
    }

    bucket_size = hinit->bucket_size - sizeof(void *);

#if 1
    printf("bucket_size:%ld\n", hinit->bucket_size - sizeof(void *));
#endif

    start = nelts / (bucket_size / (2 * sizeof(void *)));

#if 1
#endif

    start = start ? start : 1;

    // @TODO
    if (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    for (size = start; size < hinit->max_size; size++) {

        memzero(test, size * sizeof(u_short));

        for (n = 0; n < nelts; n++) {
            if (names[n].key.data == NULL) {
                continue;
            }

            //  计算真实KEY值
            key = names[n].key_hash % size;
            test[key] = (u_short) (test[key] + HASH_ELT_SIZE(&names[n]));

#if 1
            printf("%lu: %lu %u \"%s\"\n", size, key, test[key], names[n].key.data);
#endif

            if (test[key] > (u_short) bucket_size) {
                goto next;
            }
        }

        goto found;

    next:

        continue;
    }

    printf("could not build the %s, you should increase "
           "either %s_max_size: %i or %s_bucket_size: %i",
           hinit->name, hinit->name, hinit->max_size,
           hinit->name, hinit->bucket_size);

    free(test);

    return failed;

found:

    for (i = 0; i < size; i++) {
        test[i] = sizeof(void *);
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        test[key] = (u_short) (test[key] + HASH_ELT_SIZE(&names[n]));
        printf("!!!test[%lu]:%u\n", key, test[key]);
    }

    len = 0;

    for (i = 0; i < size; i++) {
        if (test[i] == sizeof(void *)) {
            continue;
        }

        test[i] = (u_short) (align(test[i], cacheline));

        printf("!!!test[%lu]:%u\n", i, test[i]);
        len += test[i];
    }

    if (hinit->hash == NULL) {
        hinit->hash = malloc(sizeof(hash_wildcard_t) + size * sizeof(hash_elt_t *));
        if (hinit->hash == NULL) {
            free(test);
            return failed;
        }

        buckets = (hash_elt_t **)
                      ((u_char *) hinit->hash + sizeof(hash_wildcard_t));

    } else {
        buckets = malloc(size * sizeof(hash_elt_t *));
        if (buckets == NULL) {
            free(test);
            return failed;
        }
    }

    elts = malloc(len + cacheline);
    if (elts == NULL) {
        free(test);
        return failed;
    }

    elts = align_ptr(elts, cacheline);

    for (i = 0; i < size; i++) {
        if (test[i] == sizeof(void *)) {
            continue;
        }

        buckets[i] = (hash_elt_t *) elts;
        elts += test[i];
        printf("test[%lu]:%u\n", i, test[i]);

    }

    for (i = 0; i < size; i++) {
        test[i] = 0;
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        elt = (hash_elt_t *) ((u_char *) buckets[key] + test[key]);

        elt->value = names[n].value;
        elt->len = (u_short) names[n].key.len;

        strlow(elt->name, names[n].key.data, names[n].key.len);

        test[key] = (u_short) (test[key] + HASH_ELT_SIZE(&names[n]));
    }

    for (i = 0; i < size; i++) {
        if (buckets[i] == NULL) {
            continue;
        }

        elt = (hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->value = NULL;
    }

    free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#if 1

    for (i = 0; i < size; i++) {
        string_t   val;
        int        key;

        elt = buckets[i];

        if (elt == NULL) {
//            printf("%ui: NULL", i);
            continue;
        }

        while (elt->value) {
            val.len = elt->len;
            val.data = &elt->name[0];

            key = hinit->key(val.data, val.len);

//            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
//                          "%ui: %p \"%V\" %ui", i, elt, &val, key);

            elt = (hash_elt_t *) align_ptr(&elt->name[0] + elt->len,
                                                   sizeof(void *));
        }
    }

#endif

    return success;
}
