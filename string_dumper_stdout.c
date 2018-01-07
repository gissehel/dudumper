#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "str_utils.h"
#include "string_dumper.h"
#include "string_dumper_stdout.h"

void string_dumper_stdout_dump(struct string_dumper* string_dumper, const char *format, ... ) {
    va_list arglist;

    va_start( arglist, format );
    vprintf( format, arglist );
    va_end( arglist );
}

void string_dumper_stdout_close(struct string_dumper* string_dumper) {
}

struct string_dumper* string_dumper_stdout_create() {
    struct string_dumper* result = malloc(sizeof(struct string_dumper()));
    result->dump = string_dumper_stdout_dump;
    result->close = string_dumper_stdout_close;
    return result;
}


