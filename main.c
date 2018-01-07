#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_info.h"
#include "node_parser.h"
#include "node_parser_json.h"
#include "node_parser_sqlite.h"
#include "str_utils.h"
#include "string_dumper_file.h"
#include "string_dumper_stdout.h"

char* append_extension(const char* base_filename, const char* extension) {
    int bufferlen = strlen(base_filename)+strlen(extension)+1+1;
    char* buffer = malloc(sizeof(char)*bufferlen);
    snprintf(buffer, bufferlen, "%s.%s", base_filename, extension);
    return buffer;
}

int main(int argc, char** argv) {
    char* path;
    struct string_dumper* string_dumper = NULL;
    struct node_parser* node_parser = NULL;

    char* output_filename = "z-output";
    char* buffer_output_filename = NULL;
    bool use_json = true;
    bool use_sqlite = true;

    if (argc < 2) {
        path = ".";
    } else {
        path = argv[1];
    }

    if (output_filename != NULL) {
        if (use_json) {
            buffer_output_filename = append_extension(output_filename, "json");
        } else if (use_sqlite) {
            buffer_output_filename = append_extension(output_filename, "sql");
        }
    }

    if (buffer_output_filename == NULL) {
        string_dumper = string_dumper_stdout_create();
    } else {
        string_dumper = string_dumper_file_create(buffer_output_filename);
    }

    if (buffer_output_filename != NULL) {
        free(buffer_output_filename);
    }


    if (use_json) {
        node_parser = node_parser_json_create( string_dumper );
    } else if (use_sqlite) {
        node_parser = node_parser_sqlite_create( string_dumper );
    }

    node_parser_parse(node_parser, path);

    node_parser->dispose(node_parser);

    return 0;
}

