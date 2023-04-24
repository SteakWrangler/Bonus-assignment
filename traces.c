#include <stdio.h>
#include <stdlib.h>

#define CACHE_SIZE 32

typedef struct {
    int tag;
    int last_used;
} cache_line;

int num_hits = 0;
int num_accesses = 0;

// Implements the least recently used replacement policy
int find_lru_index(cache_line cache[], int num_lines) {
    int lru_index = 0;
    for (int i = 0; i < num_lines; i++) {
        if (cache[i].last_used < cache[lru_index].last_used) {
            lru_index = i;
        }
    }
    return lru_index;
}

int main() {
    // Open input file
    FILE* fp = fopen("traces.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    // Initialize caches
    cache_line direct_mapped_cache[CACHE_SIZE];
    cache_line two_way_cache[CACHE_SIZE / 2][2];
    cache_line four_way_cache[CACHE_SIZE / 4][4];
    cache_line fully_associative_cache[CACHE_SIZE];

    for (int i = 0; i < CACHE_SIZE; i++) {
        direct_mapped_cache[i].tag = -1;
        fully_associative_cache[i].tag = -1;
    }
    for (int i = 0; i < CACHE_SIZE / 2; i++) {
        two_way_cache[i][0].tag = -1;
        two_way_cache[i][1].tag = -1;
    }
    for (int i = 0; i < CACHE_SIZE / 4; i++) {
        four_way_cache[i][0].tag = -1;
        four_way_cache[i][1].tag = -1;
        four_way_cache[i][2].tag = -1;
        four_way_cache[i][3].tag = -1;
    }

    // Read input file and simulate caches
    char op;
    int address;
    while (fscanf(fp, "%c %x\n", &op, &address) == 2) {
        num_accesses++;

        int direct_mapped_index = address % CACHE_SIZE;
        int two_way_set_index = (address / 2) % (CACHE_SIZE / 2);
        int four_way_set_index = (address / 4) % (CACHE_SIZE / 4);
        int fully_associative_index = -1;

        // Search direct-mapped cache
        if (direct_mapped_cache[direct_mapped_index].tag == (address / CACHE_SIZE)) {
            num_hits++;
        } else {
            direct_mapped_cache[direct_mapped_index].tag = address / CACHE_SIZE;
        }

        // Search 2-way cache
        if (two_way_cache[two_way_set_index][0].tag == (address / CACHE_SIZE)) {
            num_hits++;
            two_way_cache[two_way_set_index][0].last_used = num_accesses;
        } else if (two_way_cache[two_way_set_index][1].tag == (address / CACHE_SIZE)) {
            num_hits++;
            two_way_cache[two_way_set_index][1].last_used = num_accesses;
        } else {
            int lru_index = find_lru_index(two_way_cache[two_way_set_index], 2);
            two_way_cache[two_way_set_index][lru_index].tag = address / CACHE_SIZE;
            two_way_cache[two_way_set_index][lru_index].last_used = num_accesses;
        }

        // Search 4-way cache
}
}
