#ifndef H_BHASH_INCLUDED
#define H_BHASH_INCLUDED

#include "persist.h"

typedef int (*hash_key_pt) (u_char *data, size_t len);

typedef struct hash_init_s              hash_init_t;
typedef hash_init_t *                   hash_init_p;

typedef struct hash_s                   hash_t;
typedef hash_t *                        hash_p;

typedef struct hash_elt_s               hash_elt_t;
typedef hash_elt_t *                    hash_elt_p;

typedef struct hash_key_s               hash_key_t;
typedef hash_key_t *                    hash_key_p;

typedef struct hash_wildcard_s          hash_wildcard_t;
typedef hash_wildcard_t *               hahs_wildcard_p;

#define HASH_ELT_SIZE(name)             (sizeof(void *) + align((name)->key.len + 2, sizeof(void *)))
#define hash(key, c)                    ((int) key * 31 + c)

//  初始化结构
struct hash_init_s {
    hash_t           *hash;
    hash_key_pt       key;

    int               max_size;             //  元素个数
    int               bucket_size;          //  空间大小

    char             *name;                 //  HASH  名字
};

//  主结构
struct hash_s {
    hash_elt_t  **buckets;
    int           size;
};

struct hash_elt_s {
    void             *value;
    u_short           len;
    u_char            name[1];
} ;

//  KEY
struct hash_key_s {
    string_t          key;
    int               key_hash;
    void             *value;
};

//  通配符
struct hash_wildcard_s {
    hash_t            hash;
    void             *value;
};

int hash_key(u_char *data, size_t len);
int hash_key_lc(u_char *data, size_t len);
void * hash_find(hash_t *hash, int key, u_char *name, size_t len);
int hash_init(hash_init_p hinit, hash_key_p names, int nelts);

#endif
