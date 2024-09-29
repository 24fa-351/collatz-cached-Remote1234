// By:Eymard Alarcon //

#ifndef CACHE_H
#define CACHE_H

#define CACHE_NONE 0
#define CACHE_LRU 1
#define CACHE_RR 2
#define CACHE_ARC 3

typedef struct CacheNode {
    int key;
    int value;
    struct CacheNode *next, *prev;
} CacheNode;

typedef struct Cache {
    int size, current_size;
    CacheNode *head, *tail;
    int policy;
    CacheNode **hash_map;
} Cache;

Cache *init_cache(int size, int policy);
int get_cached_value(Cache *cache, int key);
void put_cache_value(Cache *cache, int key, int value);
void update_cache(Cache *cache, CacheNode *node);
void evict_cache_node(Cache *cache);
void free_cache(Cache *cache);

#endif
