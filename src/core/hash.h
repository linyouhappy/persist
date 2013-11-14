#ifndef H_CORE_HASH_INCLUDED
#define H_CORE_HASH_INCLUDED

#include "core/core.h"

typedef int (*hash_key_pt) (u_char *data, size_t len);

typedef struct hash_init_s              hash_init_t;
typedef hash_init_t *                   hash_init_p;

typedef struct hash_s                   hash_t;
typedef hash_t *                        hash_p;

typedef struct hash_elt_s               hash_elt_t;
typedef hash_elt_t *                    hash_elt_p;
typedef hash_elt_p *                    hash_elt_pp;

typedef struct hash_key_s               hash_key_t;
typedef hash_key_t *                    hash_key_p;

typedef struct hash_wildcard_s          hash_wildcard_t;
typedef hash_wildcard_t *               hahs_wildcard_p;

/*
 * 计算ELT结构实际大小
 */
#define HASH_ELT_SIZE(name)             (sizeof(void *) + align((name)->key.len + 2, sizeof(void *)))
#define hash(key, c)                    ((int) key * 31 + c)

//  初始化结构
struct hash_init_s {
    hash_p            hash;
    hash_key_pt       key;

    int               max_size;                 //  元素个数
    int               bucket_size;              //  空间大小

    char             *name;                     //  HASH  名字
};

//  KEY
struct hash_key_s {
    string_t          key;
    uint              hash;
    void             *value;
};

//  主结构
struct hash_s {
    hash_elt_pp       buckets;                  //  数组指针  //  存放ELT结构
    int               size;                     //  存放ELT结构的数量
};

struct hash_elt_s {
    void             *value;                    // VAL  64bit:8B
    u_short           len;                      // 长度  64bit:2
    u_char            name[1];                  // 键值  柔性数组:不占空间
} ;

//  通配符
struct hash_wildcard_s {
    hash_t            hash;
    void             *value;        //  buckets 结尾的空指针
};

extern int core_cacheline;

int hash_key(u_char *data, size_t len);
int hash_key_lc(u_char *data, size_t len);
void * hash_find(hash_t *hash, int key, u_char *name, size_t len);
int hash_init(hash_init_p hinit, hash_key_p names, int nelts);

#endif
