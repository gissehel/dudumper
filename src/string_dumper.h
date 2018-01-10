#ifndef __STRING_DUMPER_H
#define __STRING_DUMPER_H

struct string_dumper {
    void (*dump)(const struct string_dumper* string_dumper, const char *format, ... );
    void (*close)(struct string_dumper* string_dumper);
};

void string_dumper_free(struct string_dumper* string_dumper);

#endif /* __STRING_DUMPER_H */

