#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_line_parser.h"
#include "global_configuration.h"
#include "node_info.h"
#include "node_info_consumer.h"
#include "node_info_consumer_html.h"
#include "node_info_consumer_json.h"
#include "node_info_consumer_sqlite.h"
#include "node_parser.h"
#include "mem_utils.h"
#include "str_utils.h"
#include "string_dumper.h"
#include "string_dumper_file.h"
#include "string_dumper_stdout.h"

char* append_extension(const char* base_filename, const char* extension) {
    int bufferlen = strlen(base_filename)+strlen(extension)+1+1;
    MEM_CALLOC_DEF_NAME(bufferlen, char, buffer);
    snprintf(buffer, bufferlen, "%s.%s", base_filename, extension);
    return buffer;
}

int main(int argc, char** argv) {
    struct string_dumper* string_dumper = NULL;
    struct node_parser* node_parser = NULL;
    struct node_info_consumer* node_info_consumer = NULL;
    char* buffer_output_filename = NULL;
    struct global_configuration* global_configuration = command_line_parser_parse(argc, argv);
    int result=0;

    if (global_configuration->parsing_error != NULL) {
        printf("%s\n", global_configuration->parsing_error);
        result = 1;
    } else {
        if (global_configuration->need_help) {
            command_line_parser_get_help(global_configuration);
            result = 0;
        } else {
            if (global_configuration->output_filename_base != NULL) {
                if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_HTML) {
                    buffer_output_filename = append_extension(global_configuration->output_filename_base, "html");
                } else if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_JSON) {
                    buffer_output_filename = append_extension(global_configuration->output_filename_base, "json");
                } else if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_SQLITE) {
                    buffer_output_filename = append_extension(global_configuration->output_filename_base, "sql");
                }
            }

            if (buffer_output_filename == NULL) {
                string_dumper = string_dumper_stdout_create();
            } else {
                string_dumper = string_dumper_file_create(buffer_output_filename);
            }

            MEM_SET_NULL_AND_FREE(buffer_output_filename);
            
            if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_HTML) {
                node_info_consumer = node_info_consumer_html_create( string_dumper );
            } else if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_JSON) {
                node_info_consumer = node_info_consumer_json_create( string_dumper );
            } else if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_SQLITE) {
                node_info_consumer = node_info_consumer_sqlite_create( string_dumper );
            } else {
                printf("Don't know what to do...\n");
                result = 1;
            }

            if (result == 0) {
                node_parser = node_parser_create(node_info_consumer, global_configuration->max_depth);
                node_parser_parse(node_parser, global_configuration->directory, global_configuration->display_directory, global_configuration->consider_dot_dir_as_file);
                result = 0;
            }
        }
    }

    if (node_parser != NULL) {
        node_parser_free(node_parser);
        node_parser = NULL;
    }

    if (node_info_consumer != NULL) {
        node_info_consumer_free(node_info_consumer);
        node_info_consumer = NULL;
    }

    if (string_dumper != NULL) {
        string_dumper_free(string_dumper);
        string_dumper = NULL;
    }

    if (global_configuration != NULL) {
        global_configuration_free(global_configuration);
        global_configuration = NULL;
    }

    return result;
}

