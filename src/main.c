#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_line_parser.h"
#include "global_configuration.h"
#include "node_info.h"
#include "node_parser.h"
#include "node_parser_html.h"
#include "node_parser_json.h"
#include "node_parser_sqlite.h"
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

    char* buffer_output_filename = NULL;
    struct global_configuration* global_configuration = command_line_parser_parse(argc, argv);


    if (global_configuration->parsing_error != NULL) {
        printf("%s\n", global_configuration->parsing_error);
        return 1;
    }

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

    if (buffer_output_filename != NULL) {
        MEM_FREE(buffer_output_filename);
        buffer_output_filename = NULL;
    }

    
    if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_HTML) {
        node_parser = node_parser_html_create( string_dumper );
    } else if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_JSON) {
        node_parser = node_parser_json_create( string_dumper );
    } else if (global_configuration->mode == GLOBAL_CONFIGURATION_PARSER_MODE_SQLITE) {
        node_parser = node_parser_sqlite_create( string_dumper );
    } else {
        printf("Don't know what to do...\n");
        return 1;
    }

    node_parser->depth = global_configuration->max_depth;
    node_parser_parse(node_parser, global_configuration->directory);

    if (node_parser != NULL) {
        node_parser_free(node_parser);
        node_parser = NULL;
    }

    if (string_dumper != NULL) {
        string_dumper_free(string_dumper);
        string_dumper = NULL;
    }

    if (global_configuration != NULL) {
        global_configuration_free(global_configuration);
        global_configuration = NULL;
    }

    return 0;
}

