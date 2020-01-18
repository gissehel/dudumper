#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mem_utils.h"
#include "node_info.h"
#include "node_info_consumer.h"
#include "node_info_consumer_sqlite.h"
#include "str_utils.h"
#include "string_dumper.h"

struct node_info_consumer_sqlite {
    struct node_info_consumer parent;
    struct string_dumper* string_dumper;
};

void node_info_consumer_sqlite_start(const struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_sqlite, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_DUMP(string_dumper, "BEGIN TRANSACTION;\n");
        STRING_DUMPER_DUMP(string_dumper, "CREATE TABLE IF NOT EXISTS Files(id PRIMARY KEY ON CONFLICT FAIL,name, path, depth, parent_id, size, occ_size, is_dir);\n");
        STRING_DUMPER_DUMP(string_dumper, "CREATE INDEX IF NOT EXISTS index_Files_path ON Files (path);\n");
        STRING_DUMPER_DUMP(string_dumper, "CREATE INDEX IF NOT EXISTS index_Files_parent_id ON Files (id, parent_id);\n");
    }
}

void node_info_consumer_sqlite_stop(const struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_sqlite, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_DUMP(string_dumper, "COMMIT;\n");
    }
}

void node_info_consumer_sqlite_consume_node(const struct node_info_consumer* node_info_consumer, struct node_info* node_info) {
    char* name = quotestrdup_sql(node_info->name);
    char* path = quotestrdup_sql(node_info->display_path);
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_sqlite, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_DUMP(
            string_dumper,
            "INSERT INTO Files (id, name, path, depth, parent_id, size, occ_size, is_dir) VALUES (%ld, '%s', '%s', %ld, %ld, %" PRIu64 ", %" PRIu64 ", %s);\n",
            node_info->id,
            name, path,
            node_info->depth,
            node_info->parent == NULL ? 0 : node_info->parent->id,
            (uint64_t)node_info->size,
            (uint64_t)node_info->occ_size,
            node_info->is_dir ? "1" : "0"
        );
    }
    MEM_FREE(name);
    MEM_FREE(path);
}

void node_info_consumer_sqlite_dispose(struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_sqlite, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_CLOSE(string_dumper);
        MEM_STRUCT_PTR(node_info_consumer_sqlite, node_info_consumer, string_dumper) = NULL;
    }
}

struct node_info_consumer* node_info_consumer_sqlite_create(struct string_dumper* string_dumper) {
    MEM_ALLOC_STRUCT_DEF(node_info_consumer_sqlite);
    node_info_consumer_sqlite->parent.on_start = node_info_consumer_sqlite_start;
    node_info_consumer_sqlite->parent.on_stop = node_info_consumer_sqlite_stop;
    node_info_consumer_sqlite->parent.on_consume_node = node_info_consumer_sqlite_consume_node;
    node_info_consumer_sqlite->parent.on_dispose = node_info_consumer_sqlite_dispose;
    node_info_consumer_sqlite->string_dumper = string_dumper;
    return MEM_STRUCT_AS(node_info_consumer, node_info_consumer_sqlite);
}

