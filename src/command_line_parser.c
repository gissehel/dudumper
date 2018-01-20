#include <stdio.h>
#include <strings.h>
#include "command_line_parser.h"
#include "global_configuration.h"
#include "mem_utils.h"

struct global_configuration* command_line_parser_parse(int argc, char** argv) {
    struct global_configuration* global_configuration = global_configuration_create();
    if (argc >= 1) {
        global_configuration->self = MEM_STRDUP(argv[0]);
    }
    if (argc < 2) {
        global_configuration->mode = GLOBAL_CONFIGURATION_PARSER_MODE_NONE;
        global_configuration->need_help = true;
    } else {
        int arg_index;

        for (arg_index = 1;arg_index<argc;arg_index++) {
            bool is_parsed = 0;
            if (strcmp("--dir", argv[arg_index])==0) {
                arg_index++;
                if (arg_index<argc) {
                    global_configuration->directory = MEM_STRDUP(argv[arg_index]);
                    is_parsed = 1;
                } else {
                    arg_index--;
                }
            }
            if (strcmp("--out", argv[arg_index])==0) {
                arg_index++;
                if (arg_index<argc) {
                    global_configuration->output_filename_base = MEM_STRDUP(argv[arg_index]);
                    is_parsed = 1;
                } else {
                    arg_index--;
                }
            }
            if (strcmp("--help", argv[arg_index])==0) {
                global_configuration->need_help = true;
                is_parsed = 1;
            }
            if (strcmp("--html", argv[arg_index])==0) {
                global_configuration->mode = GLOBAL_CONFIGURATION_PARSER_MODE_HTML;
                is_parsed = 1;
            }
            if (strcmp("--json", argv[arg_index])==0) {
                global_configuration->mode = GLOBAL_CONFIGURATION_PARSER_MODE_JSON;
                is_parsed = 1;
            }
            if (strcmp("--sqlite", argv[arg_index])==0) {
                global_configuration->mode = GLOBAL_CONFIGURATION_PARSER_MODE_SQLITE;
                is_parsed = 1;
            }
            if (strcmp("--depth", argv[arg_index])==0) {
                arg_index++;
                if (arg_index<argc) {
                    global_configuration->max_depth = strtol(argv[arg_index], NULL, 10);
                    is_parsed = 1;
                } else {
                    arg_index--;
                }
            }

            if (! is_parsed) {
                const char* pattern = "Don't understand [%s]\n";
                int pattern_len = strlen(pattern);
                int param_len = strlen(argv[arg_index]);
                int buffer_len = pattern_len-2+param_len;
                MEM_CALLOC_DEF_NAME(buffer_len+1, char, buffer);
                snprintf(buffer, buffer_len, pattern, argv[arg_index]);
                global_configuration_add_error(global_configuration, buffer);
                MEM_FREE(buffer);
            }
        }
    }

    if (global_configuration->directory == NULL) {
        global_configuration->directory = MEM_STRDUP(".");
    }

    return global_configuration;
}

void command_line_parser_get_help(struct global_configuration* global_configuration) {
    printf("Usage: %s [OPTION]...\n", global_configuration->self);
    printf("Disk usage dumper.\n");
    printf("Dump disk usage into files that can used latter to browse,\n");
    printf("display, query disk usage.\n");
    printf("\n");
    printf("  --dir [DIRNAME]    the directory to parse\n");
    printf("  --out [FILENAME]   the base filename to parse.\n");
    printf("                     extension will be appened based\n");
    printf("                     on file type\n");
    printf("  --sqlite           dump sqlite script\n");
    printf("  --json             dump json sctructure\n");
    printf("  --html             dump html page\n");
    printf("  --help             display this help and exit\n");
    printf("\n");
}
