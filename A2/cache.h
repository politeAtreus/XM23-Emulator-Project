#pragma once
#ifndef CACHE_H
#define CACHE_H
#include "def.c"

typedef struct {
    unsigned short address; // Primary memory address
    unsigned short contents; // Cache line content
    int dirty; // Dirty bit
    int age; // For LRU
    int used;
} cache_line;

typedef enum {
    DIRECT, ASSOCIATIVE
} MappingType;

typedef enum {
    WRITE_BACK, WRITE_THROUGH
} WritePolicy;

extern cache_line cache_mem[CACHE_SIZE]; // Cache Memory
extern MappingType mapping_type;
extern WritePolicy write_policy;

extern void cache(unsigned short mar, unsigned short* mbr, int rdwr, int bw);

extern void cache_associative(unsigned short mar, unsigned short* mbr, int rdwr, int bw);

extern void cache_direct(unsigned short mar, unsigned short* mbr, int rdwr, int bw);

extern void update_usage(int cache_address);

extern void print_cache();

#endif CACHE_H