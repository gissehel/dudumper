#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "str_utils.h"
#include "mem_utils.h"
#include "string_dumper.h"
#include "string_dumper_stdout.h"

void string_dumper_stdout_dump(const struct string_dumper* string_dumper, const char *format, ... ) {
    UNUSED(string_dumper);

    va_list arglist;

    va_start( arglist, format );
    vprintf( format, arglist );
    va_end( arglist );
}

void string_dumper_stdout_close(struct string_dumper* string_dumper) {
    UNUSED(string_dumper);
}

struct string_dumper* string_dumper_stdout_create() {
    MEM_ALLOC_STRUCT_DEF(string_dumper);
    string_dumper->dump = string_dumper_stdout_dump;
    string_dumper->close = string_dumper_stdout_close;
    return string_dumper;
}


