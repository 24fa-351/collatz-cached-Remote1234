// By: Eymard Alarcon //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cache.h"
#include "collatz.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s N MIN MAX [cache_policy cache_size]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);
    if (n <= 0 || min <= 0 || max < min) {
        printf("Invalid arguments\n");
        return 1;
    }

    int cache_policy = CACHE_NONE, cache_size = 0;
    if (argc >= 6) {
        if (strcmp(argv[4], "LRU") == 0)
            cache_policy = CACHE_LRU;
        else if (strcmp(argv[4], "RR") == 0)
            cache_policy = CACHE_RR;
        else if (strcmp(argv[4], "ARC") == 0)
            cache_policy = CACHE_ARC;
        else {
            printf("Invalid cache policy. Available policies: LRU, RR, ARC\n");
            return 1;
        }
        
        cache_size = atoi(argv[5]);
        if (cache_size <= 0) {
            printf("Cache size must be a positive integer.\n");
            return 1;
        }
    }

    Cache *cache = NULL;
    if (cache_policy != CACHE_NONE)
        cache = init_cache(cache_size, cache_policy);

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int rn = min + rand() % (max - min + 1);
        int steps = get_cached_value(cache, rn);
        if (steps == -1) {
            steps = collatz_steps(rn);
            if (cache)
                put_cache_value(cache, rn, steps);
        }
        printf("%d,%d\n", rn, steps);
    }

    free_cache(cache);

    return 0;
}
