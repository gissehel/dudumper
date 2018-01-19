#ifndef __GLOBAL_CONFIGURATION_H
#define __GLOBAL_CONFIGURATION_H
#include <stdbool.h>

enum global_configuration_parser_mode {
    GLOBAL_CONFIGURATION_PARSER_MODE_NONE,
    GLOBAL_CONFIGURATION_PARSER_MODE_SQLITE,
    GLOBAL_CONFIGURATION_PARSER_MODE_JSON,
    GLOBAL_CONFIGURATION_PARSER_MODE_HTML,
};

struct global_configuration {
    enum global_configuration_parser_mode mode;
    char* output_filename_base;
    char* directory;
    long max_depth;

    char* parsing_error;
};

struct global_configuration* global_configuration_create();
void global_configuration_add_error(struct global_configuration* global_configuration, char* error_message);
void global_configuration_free(struct global_configuration*);

#endif /* __GLOBAL_CONFIGURATION_H */
