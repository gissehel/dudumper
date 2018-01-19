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

#define MEM_ALLOC(type) (mem_alloc(sizeof(type)))
#define MEM_CALLOC(size, type) (mem_alloc(size * sizeof(type)))
#define MEM_ALLOC_STRUCT(type) MEM_ALLOC(struct type)
#define MEM_ALLOC_STRUCT_DEF_NAME(type, name) struct type* name = MEM_ALLOC_STRUCT(type)
#define MEM_ALLOC_STRUCT_DEF(type) MEM_ALLOC_STRUCT_DEF_NAME(type, type)
#define MEM_STRDUP(obj) (mem_strdup(obj))
#define MEM_FREE(obj) (mem_free(obj))

#define MEM_STRUCT_AS(type, var) ((struct type*)var)
#define MEM_STRUCT_PTR(type, var, field) MEM_STRUCT_AS(type, var)->field
#define MEM_STRUCT_PTR_DEF_NAME(type, var, field, fieldtype, name) fieldtype name = MEM_STRUCT_PTR(type, var, field)
#define MEM_STRUCT_PTR_DEF(type, var, field, fieldtype) MEM_STRUCT_PTR_DEF_NAME(type, var, field, fieldtype, field)
#define MEM_STRUCT_PTR_DEF_STRUCT_NAME(type, var, field, fieldtype, name) struct fieldtype* name = MEM_STRUCT_PTR(type, var, field)
#define MEM_STRUCT_PTR_DEF_STRUCT(type, var, field, fieldtype) MEM_STRUCT_PTR_DEF_STRUCT_NAME(type, var, field, fieldtype, field)

#endif /* __MEM_UTILS_H */

