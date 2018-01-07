#ifndef __STRING_DUMPER_H
#define __STRING_DUMPER_H


struct string_dumper {
    void (*dump)(struct string_dumper* string_dumper, const char *format, ... );
    void (*close)(struct string_dumper* string_dumper);
};

#endif /* __STRING_DUMPER_H */

