#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mem_utils.h"
#include "node_info.h"
#include "node_info_consumer.h"
#include "node_info_consumer_json.h"
#include "str_utils.h"
#include "string_dumper.h"

struct node_info_consumer_json {
    struct node_info_consumer parent;
    struct string_dumper* string_dumper;
};

void node_info_consumer_json_start(const struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_json, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_DUMP(string_dumper, "[\n");
    }
}

void node_info_consumer_json_stop(const struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_json, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_DUMP(string_dumper, "{}]\n");
    }
}
 
void node_info_consumer_json_consume_node(const struct node_info_consumer* node_info_consumer, struct node_info* node_info) {
    char* name = doublequotestrdup_backquote(node_info->name);
    char* path = doublequotestrdup_backquote(node_info->display_path);
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_json, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_DUMP(
            string_dumper,
            "{\"id\":%ld, \"name\":\"%s\", \"path\":\"%s\", \"depth\":%ld, \"parent_id\":%ld, \"size\":%" PRIu64 ", \"occ_size\":%" PRIu64 ", \"is_dir\":%s},\n",
            node_info->id,
            name,
            path,
            node_info->depth,
            node_info->parent == NULL ? 0 : node_info->parent->id,
            (uint64_t)(node_info->size),
            (uint64_t)(node_info->occ_size),
            node_info->is_dir ? "true" : "false"
        );
    }
    MEM_FREE(name);
    MEM_FREE(path);
}

void node_info_consumer_json_dispose(struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_json, node_info_consumer, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        STRING_DUMPER_CLOSE(string_dumper);
        MEM_STRUCT_PTR(node_info_consumer_json, node_info_consumer, string_dumper) = NULL;
    }
}

struct node_info_consumer* node_info_consumer_json_create(struct string_dumper* string_dumper) {
    MEM_ALLOC_STRUCT_DEF(node_info_consumer_json);
    node_info_consumer_json->parent.on_start = node_info_consumer_json_start;
    node_info_consumer_json->parent.on_stop = node_info_consumer_json_stop;
    node_info_consumer_json->parent.on_consume_node = node_info_consumer_json_consume_node;
    node_info_consumer_json->parent.on_dispose = node_info_consumer_json_dispose;
    node_info_consumer_json->string_dumper = string_dumper;
    return MEM_STRUCT_AS(node_info_consumer, node_info_consumer_json);
}

