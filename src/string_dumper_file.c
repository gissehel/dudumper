#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "str_utils.h"
#include "mem_utils.h"
#include "string_dumper.h"
#include "string_dumper_file.h"

struct string_dumper_file {
    struct string_dumper parent;
    FILE* handle;
};

void string_dumper_file_dump(const struct string_dumper* string_dumper, const char *format, ... ) {
    MEM_STRUCT_PTR_DEF(string_dumper_file, string_dumper, handle, FILE*);
    if (handle != NULL) {
        va_list arglist;

        va_start( arglist, format );
        vfprintf( handle, format, arglist );
        va_end( arglist );
    }
}

void string_dumper_file_close(struct string_dumper* string_dumper) {
    MEM_STRUCT_PTR_DEF(string_dumper_file, string_dumper, handle, FILE*);
    if (handle != NULL) {
        fclose(handle);
    }
    MEM_STRUCT_PTR(string_dumper_file, string_dumper, handle) = NULL;
}

struct string_dumper* string_dumper_file_create(const char* filename) {
    MEM_ALLOC_STRUCT_DEF(string_dumper_file);
    string_dumper_file->handle = fopen(filename, "wb");
    string_dumper_file->parent.dump = string_dumper_file_dump;
    string_dumper_file->parent.close = string_dumper_file_close;
    return MEM_STRUCT_AS(string_dumper, string_dumper_file);
}


