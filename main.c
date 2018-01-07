#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>

#include "node_info.h"
#include "node_parser.h"
#include "node_parser_json.h"
#include "node_parser_sqlite.h"
#include "str_utils.h"
#include "string_dumper_file.h"
#include "string_dumper_stdout.h"

int main(int argc, char** argv) {
    char* path;
    struct string_dumper* string_dumper = NULL;
    struct node_parser* node_parser = NULL;

    char* output_filename = "z-output";
    bool use_json = true;
    bool use_sqlite = true;

    if (argc < 2) {
        path = ".";
    } else {
        path = argv[1];
    }

    if (output_filename == NULL) {
        string_dumper = string_dumper_stdout_create();
    } else {
        string_dumper = string_dumper_file_create(output_filename);
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

