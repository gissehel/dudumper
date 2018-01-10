#ifndef __MEM_UTILS_H
#define __MEM_UTILS_H
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG

// void* mem_alloc(size_t size);
// void mem_free(void* ptr);
// char *mem_strdup(const char *s);

void* mem_alloc_fl(size_t size, const char* filename, long line);
void mem_free_fl(void* ptr, const char* filename, long line);
char *mem_strdup_fl(const char *s, const char* filename, long line);

#define mem_alloc(x) mem_alloc_fl(x, __FILE__, __LINE__)
#define mem_free(x) mem_free_fl(x, __FILE__, __LINE__)
#define mem_strdup(x) mem_strdup_fl(x, __FILE__, __LINE__)


#else

#define mem_alloc(x) malloc(x)
#define mem_free(x) free(x)
#define mem_strdup(x) strdup(x)

#endif


#endif /* __MEM_UTILS_H */

