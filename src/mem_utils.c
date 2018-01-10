#include "mem_utils.h"

#ifdef DEBUG

#include <stdio.h>

static unsigned long mem_event_count = 0;

void* mem_alloc_fl(size_t size, const char* filename, long line) {
    void* result = malloc(size);
    printf("*+[%p]:[%lu] Allocating [%zu] at [%p] (%s:%lu)\n", result, mem_event_count++, size, result, filename, line);
    return result;
}

void mem_free_fl(void* ptr, const char* filename, long line) {
    printf("*-[%p]:[%lu] Freeing memory at [%p] (%s:%lu)\n", ptr, mem_event_count++, ptr, filename, line);
    free(ptr);
}

char* mem_strdup_fl(const char *s, const char* filename, long line) {
    char* result = strdup(s);
    printf("*+[%p]:[%lu] Strduping memory at [%p] (%s:%lu)\n", result, mem_event_count++, result, filename, line);
    return result;
}

#endif

