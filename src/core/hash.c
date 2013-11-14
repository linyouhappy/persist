#include "core/hash.h"

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
   printf("hf:\"%*s\"", len, name);
#endif

    //  计算KEY所在的ELT起始位置
    elt = hash->buckets[key % hash->size];

    if (elt == NULL) {
        return NULL;
    }

    while (elt->value) {
        //  判断长度
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

        elt = (hash_elt_t *) align_ptr(&elt->name[0] + elt->len, sizeof(void *));
        continue;
    }

    return NULL;
}

int hash_init(hash_init_p hinit, hash_key_p names, int nelts) {
    //  检查CACHELINE是否初始化
    if (core_cacheline == 0) {
        core_cpuinfo();
    }

    u_char          *elts;
    size_t           len;
    u_short         *test;
    uint            i, n, key, size, start, bucket_size;
    hash_elt_t      *elt, **buckets;

    for (n = 0; n < nelts; n++) {
        if (hinit->bucket_size < HASH_ELT_SIZE(&names[n]) + sizeof(void *)) {
            printf("%ld %lu\n", names[n].key.len, HASH_ELT_SIZE(&names[n]));
            printf("could not build the %s, you should "
                   "increase %s_bucket_size: %i",
                    hinit->name, hinit->name, hinit->bucket_size);
            return failed;
        }
    }

    test = malloc(hinit->max_size * sizeof(u_short));

    if (test == NULL) {
        return failed;
    }

    //  每个BUCKET后面需要一个NULL指针数据
    bucket_size = hinit->bucket_size - sizeof(void *);


    /*
     *  2*sizeof(void*) : hash_elt_t
     */
    start = nelts / (bucket_size / sizeof(hash_elt_t)); //(2 * sizeof(void *)));

    start = start ? start : 1;

    // @TODO
    if (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    // 这里计算的是KEY长度
    for (size = start; size < hinit->max_size; size++) {

        memzero(test, size * sizeof(u_short));

        for (n = 0; n < nelts; n++) {
            if (names[n].key.data == NULL) {
                continue;
            }

            //  计算真实KEY值
            key = names[n].hash % size;
            test[key] = (u_short) (test[key] + HASH_ELT_SIZE(&names[n]));

            printf("%u %lu (%lu)\"%s\"\n", key, HASH_ELT_SIZE(&names[n]), names[n].key.len, names[n].value);

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

    //  重置TEST变量
    for (i = 0; i < size; i++) {
        test[i] = sizeof(void *);
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].hash % size;
        test[key] = (u_short) (test[key] + HASH_ELT_SIZE(&names[n]));
    }

    len = 0;

    for (i = 0; i < size; i++) {
        if (test[i] == sizeof(void *)) {
            continue;
        }

        test[i] = (u_short) (align(test[i], core_cacheline));

        len += test[i];
    }

    if (hinit->hash == NULL) {
        hinit->hash = malloc(sizeof(hash_wildcard_t) + size * sizeof(hash_elt_p));
        if (hinit->hash == NULL) {
            free(test);
            return failed;
        }

        buckets = (hash_elt_pp) ((u_char *) hinit->hash + sizeof(hash_wildcard_t));

    } else {
        buckets = malloc(size * sizeof(hash_elt_p));
        if (buckets == NULL) {
            free(test);
            return failed;
        }
    }

    elts = malloc(len + core_cacheline);
    if (elts == NULL) {
        free(test);
        return failed;
    }

    elts = align_ptr(elts, core_cacheline);

    for (i = 0; i < size; i++) {
        if (test[i] == sizeof(void *)) {
            continue;
        }

        buckets[i] = (hash_elt_t *) elts;
        elts += test[i];

    }

    for (i = 0; i < size; i++) {
        test[i] = 0;
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].key.data == NULL) {
            continue;
        }

        //  计算BUCKETS下的标记
        key = names[n].hash % size;
        //  获取当前数据写入的位置 : buckets[key] key起始位置， test[key] 已使用的大小
        elt = (hash_elt_t *) ((u_char *) buckets[key] + test[key]);

        elt->value = names[n].value;
        elt->len   = (u_short) names[n].key.len;

        strlow(elt->name, names[n].key.data, names[n].key.len);

        //  记录已使用大小 (字节对齐后的大小)
        test[key] = (u_short) (test[key] + HASH_ELT_SIZE(&names[n]));
    }

    //  为BUCKETS结尾补一个NULL(空指针)
    for (i = 0; i < size; i++) {
        if (buckets[i] == NULL) {
            continue;
        }

        elt = (hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->value = NULL;
    }

    free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size    = size;
    return success;
}
