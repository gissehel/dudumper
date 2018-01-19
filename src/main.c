#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char* buffer = MEM_CALLOC(bufferlen, char);
    snprintf(buffer, bufferlen, "%s.%s", base_filename, extension);
    return buffer;
}

int main(int argc, char** argv) {
    char* path = NULL;
    struct string_dumper* string_dumper = NULL;
    struct node_parser* node_parser = NULL;

    char* output_filename = NULL;
    char* buffer_output_filename = NULL;
    bool use_html = false;
    bool use_json = false;
    bool use_sqlite = false;
    long max_depth = 0;


    if (argc < 2) {
        use_html  = true;
        path = ".";
    } else {
        int arg_index;
        bool is_error = 0;

        for (arg_index = 1;arg_index<argc;arg_index++) {
            bool is_parsed = 0;
            if (strcmp("--dir", argv[arg_index])==0) {
                arg_index++;
                if (arg_index<argc) {
                    path = argv[arg_index];
                    is_parsed = 1;
                } else {
                    arg_index--;
                }
            }
            if (strcmp("--out", argv[arg_index])==0) {
                arg_index++;
                if (arg_index<argc) {
                    output_filename = argv[arg_index];
                    is_parsed = 1;
                } else {
                    arg_index--;
                }
            }
            if (strcmp("--html", argv[arg_index])==0) {
                use_html = true;
                is_parsed = 1;
            }
            if (strcmp("--json", argv[arg_index])==0) {
                use_json = true;
                is_parsed = 1;
            }
            if (strcmp("--sqlite", argv[arg_index])==0) {
                use_sqlite = true;
                is_parsed = 1;
            }
            if (strcmp("--depth", argv[arg_index])==0) {
                arg_index++;
                if (arg_index<argc) {
                    max_depth = strtol(argv[arg_index], NULL, 10);
                    is_parsed = 1;
                } else {
                    arg_index--;
                }
            }


            if (! is_parsed) {
                printf("Don't understand [%s]\n", argv[arg_index]);
                is_error = 1;
            }
        }
        if (is_error) {
            return 1;
        }
    }

    if (path == NULL) {
        path = ".";
    }

    if (output_filename != NULL) {
        if (use_html) {
            buffer_output_filename = append_extension(output_filename, "html");
        } else if (use_json) {
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
        MEM_FREE(buffer_output_filename);
        buffer_output_filename = NULL;
    }


    if (use_html) {
        node_parser = node_parser_html_create( string_dumper );
    } else if (use_json) {
        node_parser = node_parser_json_create( string_dumper );
    } else if (use_sqlite) {
        node_parser = node_parser_sqlite_create( string_dumper );
    } else {
        printf("Don't know what to do...\n");
        return 1;
    }

    node_parser->depth = max_depth;

    node_parser_parse(node_parser, path);

    node_parser_free(node_parser);
    node_parser = NULL;

    string_dumper_free(string_dumper);
    string_dumper = NULL;

    return 0;
}

