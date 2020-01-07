#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "global_configuration.h"
#include "mem_utils.h"

struct global_configuration* global_configuration_create() {
    MEM_ALLOC_STRUCT_DEF(global_configuration);
    global_configuration->self = NULL;
    global_configuration->mode = GLOBAL_CONFIGURATION_PARSER_MODE_NONE;
    global_configuration->output_filename_base = NULL;
    global_configuration->directory = NULL;
    global_configuration->parsing_error = NULL;
    global_configuration->max_depth = 0;
    global_configuration->need_help = false;
    global_configuration->consider_dot_dir_as_file = true;
    return global_configuration;
}

void global_configuration_add_error(struct global_configuration* global_configuration, char* error_message) {
    MEM_STRUCT_PTR_DEF(global_configuration, global_configuration, parsing_error, char*);
    if (parsing_error != NULL) {
        int final_error_size = strlen(parsing_error) + strlen(error_message) + 1 + 1;
        MEM_CALLOC_DEF_NAME(final_error_size, char, final_error);
        snprintf(final_error, final_error_size, "%s\n%s", parsing_error, error_message);
        MEM_FREE(parsing_error);
        parsing_error = NULL;
        MEM_STRUCT_PTR(global_configuration, global_configuration, parsing_error) = final_error;
    } else {
        MEM_STRUCT_PTR(global_configuration, global_configuration, parsing_error) = MEM_STRDUP(error_message);
    }
}

void global_configuration_free(struct global_configuration* global_configuration) {
    MEM_SET_NULL_AND_FREE(global_configuration->self);
    MEM_SET_NULL_AND_FREE(global_configuration->output_filename_base);
    MEM_SET_NULL_AND_FREE(global_configuration->directory);
    MEM_SET_NULL_AND_FREE(global_configuration->parsing_error);
    MEM_FREE(global_configuration);
}
