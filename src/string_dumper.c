#include "string_dumper.h"
#include "mem_utils.h"

void string_dumper_free(struct string_dumper* string_dumper) {
    string_dumper->close(string_dumper);
    MEM_FREE(string_dumper);
}
