#define _XOPEN_SOURCE 500
/*
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <string.h>
#include <stdbool.h>
*/
#include <stdio.h>
#include <stdlib.h>
#include "node_info.h"
#include "node_parser.h"

int __needsquotes(const char* s, int* quotelen);
char* quotestrdup(const char *s);

void on_node_parse_start() {
    printf("BEGIN TRANSACTION;\n");
    printf("CREATE TABLE IF NOT EXISTS Files(id PRIMARY KEY ON CONFLICT FAIL,name, path, depth, parent_id, size, occ_size);\n");
    printf("CREATE INDEX IF NOT EXISTS index_Files_path ON Files (path);\n");
    printf("CREATE INDEX IF NOT EXISTS index_Files_parent_id ON Files (id, parent_id);\n");
}

void on_node_parse_stop() {
    // printf("-- end\n");
    // dump_node(node_info_root, true);
    printf("COMMIT;\n");
}

void on_node_display(struct node_info* node_info_item) {
    char* name = quotestrdup(node_info_item->name);
    char* path = quotestrdup(node_info_item->path);
    printf("INSERT INTO Files (id, name, path, depth, parent_id, size, occ_size) VALUES (%ld, '%s', '%s', %ld, %ld, %ld, %ld);\n",  node_info_item->id, name, path, node_info_item->depth, node_info_item->parent == NULL ? 0 : node_info_item->parent->id, node_info_item->size, node_info_item->occ_size);
    free(name);
    free(path);
}

int main(int argc, char** argv) {
    char* path;
    if (argc < 1) {
        path = ".";
    } else {
        path = argv[1];
    }
    struct node_parse_output* output = node_parse_output_create();
    output->on_node_parse_start = on_node_parse_start;
    output->on_node_parse_stop = on_node_parse_stop;
    output->on_node_display = on_node_display;
    struct node_parse_data* data = node_parse_data_create(output);
    printf("ok\n");
    node_parse(data, path);
    return 0;
}

