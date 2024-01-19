#include "cache.h"
#include "mem.h"

extern long long clock;

int hit_count = 0;
int miss_count = 0;

void cache(unsigned short mar, unsigned short* mbr, int rdwr, int bw) {
    if (mapping_type == DIRECT) {
        cache_direct(mar, mbr, rdwr, bw);
    }
    else {
        cache_associative(mar, mbr, rdwr, bw);
    }
    clock++;
}

// The cache_direct function handles the case where the cache is organized 
// using accosiative mapping. It uses the memory address register (mar) and 
// memory buffer register (mbr) for read and write operations and the LRU algorithm
void cache_associative(unsigned short mar, unsigned short* mbr, int rdwr, int bw) {
    int i, lru_index = 0;

    for (i = 0; i < CACHE_SIZE; i++) {
        if (cache_mem[i].address == mar) {
            hit_count++;
            if (rdwr == READ) {
                *mbr = cache_mem[i].contents;
                cache_mem[i].used = 1;
            }
            else {
                cache_mem[i].contents = *mbr;
                cache_mem[i].used = 1;
                cache_mem[i].dirty = TRUE;
                if (write_policy == WRITE_THROUGH) {
                    memory_bus(mar, mbr, WRITE, bw);
                    cache_mem[i].dirty = FALSE;
                }
            }
            // update age values whenever a cache hit occurs
            update_usage(i);
            return;
        }
        if (cache_mem[i].used == 0)
            lru_index = i;
    }

    // If we reach this point, we've had a cache miss.
    // Cache miss handling code is unchanged.
    miss_count++;
    if (rdwr == READ) {
        memory_bus(mar, mbr, READ, bw);
        cache_mem[lru_index].address = mar;
        cache_mem[lru_index].contents = *mbr;
        cache_mem[lru_index].used = 1;
    }
    else {
        if (cache_mem[lru_index].dirty) {
            unsigned short old_data = cache_mem[lru_index].contents;
            memory_bus(cache_mem[lru_index].address, &old_data, WRITE, bw);
        }
        cache_mem[lru_index].address = mar;
        cache_mem[lru_index].contents = *mbr;
        cache_mem[lru_index].used = 1;
        cache_mem[lru_index].dirty = TRUE;
        if (write_policy == WRITE_THROUGH) {
            memory_bus(mar, mbr, WRITE, bw);
            cache_mem[lru_index].dirty = FALSE;
        }
    }
}


// The cache_direct function handles the case where the cache is organized 
// using direct mapping. It uses the memory address register (mar) and 
// memory buffer register (mbr) for read and write operations.
void cache_direct(unsigned short mar, unsigned short* mbr, int rdwr, int bw) {

    // Compute cache line address from the memory address by taking modulo of CACHE_SIZE
    unsigned short cache_address = mar % CACHE_SIZE;

    // If the memory address matches the address in the cache line, it is a cache hit.
    if (mar == cache_mem[cache_address].address) {
        hit_count++;
        // If it's a read operation, set mbr to the contents of the cache line.
        if (rdwr == READ) {
            *mbr = cache_mem[cache_address].contents;
        }
        // If it's a write operation, update the contents of the cache line, 
        // mark it as dirty and if the write policy is WRITE_THROUGH, also write to memory.
        else {
            cache_mem[cache_address].contents = *mbr;
            cache_mem[cache_address].dirty = TRUE;
            if (write_policy == WRITE_THROUGH) {
                memory_bus(mar, mbr, WRITE, bw);
                cache_mem[cache_address].dirty = FALSE;
            }
        }
    }
    // If the memory address doesn't match the address in the cache line, it is a cache miss.
    else {
        miss_count++;

        // If it's a read operation, fetch the memory contents, and update the cache line.
        if (rdwr == READ) {
            memory_bus(mar, mbr, READ, bw);
            cache_mem[cache_address].address = mar;
            cache_mem[cache_address].contents = *mbr;
        }
        // If it's a write operation, and the existing cache line is dirty,
        // write it back to memory. Then, update the cache line with new data 
        // and mark it as dirty. If the write policy is WRITE_THROUGH, also write to memory.
        else {
            if (cache_mem[cache_address].dirty) {
                unsigned short old_data = cache_mem[cache_address].contents;
                memory_bus(cache_mem[cache_address].address, &old_data, WRITE, bw);
            }
            cache_mem[cache_address].address = mar;
            cache_mem[cache_address].contents = *mbr;
            cache_mem[cache_address].dirty = TRUE;
            if (write_policy == WRITE_THROUGH) {
                memory_bus(mar, mbr, WRITE, bw);
                cache_mem[cache_address].dirty = FALSE;
            }
        }
    }
}

// The update_usage function updates the age of the cache lines.
// It increments the age of the accessed line and decrements the age of other lines.
void update_usage(int cache_address) {

    // Set age of the accessed line to CACHE_SIZE - 1 to indicate it was recently used.
    cache_mem[cache_address].age = CACHE_SIZE - 1;

    // Decrement the age of all other lines to make them less recently used.
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (i != cache_address && cache_mem[i].age > 0) {
            cache_mem[i].age--;
        }
    }
}

void print_cache() {
    printf("\nCACHE CONTENTS:\n");
    printf("===========================================\n");
    printf("Address\t\tContents\tDirty\tAge\n");
    printf("===========================================\n");

    for (int i = 0; i < CACHE_SIZE; i++) {
        printf("0x%04x\t\t0x%02x\t\t%d\t%d\n", cache_mem[i].address, cache_mem[i].contents, cache_mem[i].dirty, cache_mem[i].age);
    }

    printf("===========================================\n");
    printf("Hits: %d, Misses: %d\n", hit_count, miss_count);
    if (miss_count == 0 && hit_count == 0)
        printf("Hit to Miss ratio: N/A\n");
    else if (miss_count == 0)
        printf("Hit to Miss ratio: Perfect\n");
    else
        printf("Hit to Miss ratio: %.2f\n", (float)hit_count / miss_count);

    if (mapping_type == 0) {
        printf("Mapping type is set to Direct mapping.\n");
    }
    else {
        printf("Mapping type is set to Associative mapping.\n");
    }
    if (write_policy == 0) {
        printf("Write policy is set to Write back.\n");
    }
    else {
        printf("Write policy is set to Write through.\n");
    }
}