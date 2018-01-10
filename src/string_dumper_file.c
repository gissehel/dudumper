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
    FILE* handle = ((struct string_dumper_file*)string_dumper)->handle;
    if (handle != NULL) {
        va_list arglist;

        va_start( arglist, format );
        vfprintf( handle, format, arglist );
        va_end( arglist );
    }
}

void string_dumper_file_close(struct string_dumper* string_dumper) {
    FILE* handle = ((struct string_dumper_file*)string_dumper)->handle;
    if (handle != NULL) {
        fclose(handle);
    }
    ((struct string_dumper_file*)string_dumper)->handle = NULL;
}

struct string_dumper* string_dumper_file_create(const char* filename) {
    struct string_dumper_file* result = mem_alloc(sizeof(struct string_dumper_file()));
    result->handle = fopen(filename, "wb");
    result->parent.dump = string_dumper_file_dump;
    result->parent.close = string_dumper_file_close;
    return (struct string_dumper*)result;
}


