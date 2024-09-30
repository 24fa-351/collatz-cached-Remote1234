//By: Eymard Alarcon //

#include "cache.h"
#include <stdlib.h>

Cache *init_cache(int size, int policy) {
    Cache *cache = (Cache *)malloc(sizeof(Cache));
    cache->size = size;
    cache->current_size = 0;
    cache->head = cache->tail = NULL;
    cache->hash_map = (CacheNode **)calloc(500000, sizeof(CacheNode *));
    cache->policy = policy;
    return cache;
}

int get_cached_value(Cache *cache, int key) {
    if (!cache)
        return -1;

    CacheNode *node = cache->hash_map[key];
    if (node) {
        if (cache->policy == CACHE_LRU || cache->policy == CACHE_ARC) {
            update_cache(cache, node);
        }
        return node->value;
    }
    return -1;
}

void put_cache_value(Cache *cache, int key, int value) {
    if (!cache)
        return;

    if (cache->current_size >= cache->size) {
        evict_cache_node(cache);
    }

    CacheNode *node = (CacheNode *)malloc(sizeof(CacheNode));
    node->key = key;
    node->value = value;
    node->next = cache->head;
    node->prev = NULL;

    if (cache->head)
        cache->head->prev = node;
    cache->head = node;

    if (!cache->tail)
        cache->tail = node;

    cache->hash_map[key] = node;
    cache->current_size++;
}

void update_cache(Cache *cache, CacheNode *node) {
    if (node == cache->head) return;

    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;

    if (node == cache->tail) {
        cache->tail = node->prev;
        if (cache->tail) cache->tail->next = NULL;
    }

    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
}

void evict_cache_node(Cache *cache) {
    CacheNode *node_to_evict = NULL;

    switch (cache->policy) {
        case CACHE_LRU:
            node_to_evict = cache->tail;
            break;
        case CACHE_RR:
            {
                int rand_index = rand() % cache->current_size;
                CacheNode *current = cache->head;
                for (int i = 0; i < rand_index; i++) {
                    current = current->next;
                }
                node_to_evict = current;
            }
            break;
        case CACHE_ARC:
            node_to_evict = cache->tail;
            break;
        default:
            return;
    }

    if (node_to_evict) {
        if (node_to_evict->prev)
            node_to_evict->prev->next = node_to_evict->next;
        if (node_to_evict->next)
            node_to_evict->next->prev = node_to_evict->prev;

        if (node_to_evict == cache->head)
            cache->head = node_to_evict->next;
        if (node_to_evict == cache->tail)
            cache->tail = node_to_evict->prev;

        cache->hash_map[node_to_evict->key] = NULL;
        free(node_to_evict);
        cache->current_size--;
    }
}

void free_cache(Cache *cache) {
    if (!cache) return;

    CacheNode *current = cache->head;
    while (current) {
        CacheNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(cache->hash_map);
    free(cache);
}
