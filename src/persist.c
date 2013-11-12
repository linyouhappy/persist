#include "persist.h"

//static void * my_process(void * args, thread_worker_p worker) {
//    printf("worder(%d):task(%d)\n", worker->id, worker->task->id);
//    sleep(2);
//    return worker;
//}
//
//// thread_pool_demo
//static void thread_pool_demo() {
//    thread_pool_p pool;
//    pool = thread_pool_create(null);
//    thread_task_add(pool, my_process, NULL);
//    thread_pool_destroy(pool);
//}

//void _read(connection_p c) {
//    printf("%d:read\n", c->fd);
//}
//
//void _close(connection_p c) {
//    printf("%d:close\n", c->fd);
//}

int main(void) {
    //  server example
//    server_p server = server_initialize();
//    server->port = 16622;
//    server->ip   = null;
//
//    server->read  = _read;
//    server->close = _close;
//
//    server_tcp_create(server);
//    server_tcp_process();


    //  array example
//    int i;
//    array_p  array;
//    string_p string;
//    array = array_create(10, sizeof(string_t));
//
//    if (null == array) {
//        printf("array_create failed!");
//    }
//
//    string = array_push(array);
//    str_set(string, "aaa111");
//
//    string = array_push(array);
//    str_set(string, "bbb222");
//
//    for(i=0; i<array->nelts; i++) {
//        string = array->elts + i * array->size;
//
//        printf("%s\n", string->data);
//    }
//    printf("success\n");
//    exit(0);
    //  hash example

    hash_init_t     hinit;
    hash_p          hash;
    int             i;
    array_p         array;
    hash_key_p      key;
    static string_t names[] = {string("rainx"),
            string("xiaozhe"),
            string("zhoujian")};
    static char* descs[] = {
            "rainx's id is 1",
            "xiaozhe's id is 2",
            "zhoujian's id is 3"
    };

    array = array_create(10, sizeof(hash_key_t));;
    hash = malloc(sizeof(hash_t));

    hinit.hash = hash;
    hinit.key = &hash_key_lc;
    hinit.max_size = 1024*10;
    hinit.bucket_size = 64;
    hinit.name = "hash";

    for(i = 0; i < 3; i++) {
        key            = (hash_key_t*) array_push(array);
        key->key       = (names[i]);
        key->key_hash  = hash_key_lc(key->key.data, key->key.len);
        key->value     = (void*) descs[i];
        printf("key: %s ,\tkey_hash: %u\n", key->key.data, key->key_hash);
    }

    if (hash_init(&hinit, (hash_key_t*) array->elts, array->nelts) !=success){
        return 1;
    }
    printf("init success\n");

    // 查找
    int k;
    char*               find;
    k    = hash_key_lc(names[0].data, names[0].len);
    printf("%s key is %d\n", names[0].data, k);
    find = (char*) hash_find(hash, k, (u_char*) names[0].data, names[0].len);

    if (find) {
        printf("value: %s\n", (char*) find);
    }

    return 0;
}
