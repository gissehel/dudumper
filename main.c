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
#include "str_utils.h"

void on_node_parser_start_sqlite() {
    printf("BEGIN TRANSACTION;\n");
    printf("CREATE TABLE IF NOT EXISTS Files(id PRIMARY KEY ON CONFLICT FAIL,name, path, depth, parent_id, size, occ_size, is_dir);\n");
    printf("CREATE INDEX IF NOT EXISTS index_Files_path ON Files (path);\n");
    printf("CREATE INDEX IF NOT EXISTS index_Files_parent_id ON Files (id, parent_id);\n");
}

void on_node_parser_stop_sqlite() {
    // printf("-- end\n");
    // dump_node(node_info_root, true);
    printf("COMMIT;\n");
}

void on_node_display_sqlite(struct node_info* node_info_item) {
    char* name = quotestrdup_sql(node_info_item->name);
    char* path = quotestrdup_sql(node_info_item->path);
    printf("INSERT INTO Files (id, name, path, depth, parent_id, size, occ_size, is_dir) VALUES (%ld, '%s', '%s', %ld, %ld, %ld, %ld, %s);\n",  node_info_item->id, name, path, node_info_item->depth, node_info_item->parent == NULL ? 0 : node_info_item->parent->id, node_info_item->size, node_info_item->occ_size, node_info_item->is_dir ? "1" : "0");
    free(name);
    free(path);
}

struct node_parser_output* get_node_display_sqlite() {
    struct node_parser_output* output = node_parser_output_create();
    output->on_node_parser_start = on_node_parser_start_sqlite;
    output->on_node_parser_stop = on_node_parser_stop_sqlite;
    output->on_node_display = on_node_display_sqlite;
    return output;
}


void on_node_parser_start_json() {
    printf("[\n");
}

void on_node_parser_stop_json() {
    // printf("-- end\n");
    // dump_node(node_info_root, true);
    printf("{}]\n");
}

void on_node_display_json(struct node_info* node_info_item) {
    char* name = quotestrdup_backquote(node_info_item->name);
    char* path = quotestrdup_backquote(node_info_item->path);
    printf("{id:%ld, name:'%s', path:'%s', depth:%ld, parent_id:%ld, size:%ld, occ_size:%ld, is_dir:%s},\n",  node_info_item->id, name, path, node_info_item->depth, node_info_item->parent == NULL ? 0 : node_info_item->parent->id, node_info_item->size, node_info_item->occ_size, node_info_item->is_dir ? "true" : "false");
    free(name);
    free(path);
}

struct node_parser_output* get_node_display_json() {
    struct node_parser_output* output = node_parser_output_create();
    output->on_node_parser_start = on_node_parser_start_json;
    output->on_node_parser_stop = on_node_parser_stop_json;
    output->on_node_display = on_node_display_json;
    return output;
}



int main(int argc, char** argv) {
    char* path;
    if (argc < 2) {
        path = ".";
    } else {
        path = argv[1];
    }
    // struct node_parser_output* output = get_node_display_sqlite();
    struct node_parser_output* output = get_node_display_json();
    node_parse(output, path);
    return 0;
}

