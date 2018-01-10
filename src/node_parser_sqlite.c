#include <stdio.h>
#include <stdlib.h>
#include "node_info.h"
#include "node_parser.h"
#include "string_dumper.h"
#include "node_parser_sqlite.h"
#include "str_utils.h"
#include "mem_utils.h"

struct node_parser_sqlite {
    struct node_parser parent;
    struct string_dumper* string_dumper;
};

void on_node_parser_start_sqlite(const struct node_parser* node_parser) {
    const struct string_dumper* string_dumper = ((const struct node_parser_sqlite*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "BEGIN TRANSACTION;\n");
        string_dumper->dump(string_dumper, "CREATE TABLE IF NOT EXISTS Files(id PRIMARY KEY ON CONFLICT FAIL,name, path, depth, parent_id, size, occ_size, is_dir);\n");
        string_dumper->dump(string_dumper, "CREATE INDEX IF NOT EXISTS index_Files_path ON Files (path);\n");
        string_dumper->dump(string_dumper, "CREATE INDEX IF NOT EXISTS index_Files_parent_id ON Files (id, parent_id);\n");
    }
}

void on_node_parser_stop_sqlite(const struct node_parser* node_parser) {
    // printf("-- end\n");
    // dump_node(node_info_root, true);
    struct string_dumper* string_dumper = ((struct node_parser_sqlite*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "COMMIT;\n");
    }
}

void on_node_display_sqlite(const struct node_parser* node_parser, struct node_info* node_info_item) {
    char* name = quotestrdup_sql(node_info_item->name);
    char* path = quotestrdup_sql(node_info_item->path);
    struct string_dumper* string_dumper = ((struct node_parser_sqlite*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "INSERT INTO Files (id, name, path, depth, parent_id, size, occ_size, is_dir) VALUES (%ld, '%s', '%s', %ld, %ld, %ld, %ld, %s);\n",  node_info_item->id, name, path, node_info_item->depth, node_info_item->parent == NULL ? 0 : node_info_item->parent->id, node_info_item->size, node_info_item->occ_size, node_info_item->is_dir ? "1" : "0");
    }
    mem_free(name);
    mem_free(path);
}

void node_parser_sqlite_dispose(struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_sqlite*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->close(string_dumper);
        ((struct node_parser_sqlite*)node_parser)->string_dumper = NULL;
    }
}

struct node_parser* node_parser_sqlite_create(struct string_dumper* string_dumper) {
    struct node_parser_sqlite* node_parser_sqlite = mem_alloc(sizeof(struct node_parser_sqlite));
    node_parser_sqlite->parent.depth = 0;
    node_parser_sqlite->parent.on_node_parser_start = on_node_parser_start_sqlite;
    node_parser_sqlite->parent.on_node_parser_stop = on_node_parser_stop_sqlite;
    node_parser_sqlite->parent.on_node_display = on_node_display_sqlite;
    node_parser_sqlite->parent.dispose = node_parser_sqlite_dispose;
    node_parser_sqlite->string_dumper = string_dumper;
    return (struct node_parser*)node_parser_sqlite;
}

