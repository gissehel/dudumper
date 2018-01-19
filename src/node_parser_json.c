#include <stdio.h>
#include <stdlib.h>
#include "node_info.h"
#include "node_parser.h"
#include "string_dumper.h"
#include "node_parser_json.h"
#include "str_utils.h"
#include "mem_utils.h"
#include <inttypes.h>

struct node_parser_json {
    struct node_parser parent;
    struct string_dumper* string_dumper;
};


void on_node_parser_start_json(const struct node_parser* node_parser) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser_json, node_parser, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "[\n");
    }
}

void on_node_parser_stop_json(const struct node_parser* node_parser) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser_json, node_parser, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "{}]\n");
    }
}

void on_node_display_json(const struct node_parser* node_parser, struct node_info* node_info_item) {
    char* name = doublequotestrdup_backquote(node_info_item->name);
    char* path = doublequotestrdup_backquote(node_info_item->path);
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser_json, node_parser, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        string_dumper->dump(
            string_dumper, 
            "{\"id\":%ld, \"name\":\"%s\", \"path\":\"%s\", \"depth\":%ld, \"parent_id\":%ld, \"size\":%" PRIu64 ", \"occ_size\":%" PRIu64 ", \"is_dir\":%s},\n",  
            node_info_item->id, 
            name, 
            path, 
            node_info_item->depth, 
            node_info_item->parent == NULL ? 0 : node_info_item->parent->id,
            (uint64_t)(node_info_item->size), 
            (uint64_t)(node_info_item->occ_size), 
            node_info_item->is_dir ? "true" : "false"
        );
    }
    MEM_FREE(name);
    MEM_FREE(path);
}

void node_parser_json_dispose(struct node_parser* node_parser) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser_json, node_parser, string_dumper, string_dumper);
    if (string_dumper != NULL) {
        string_dumper->close(string_dumper);
        MEM_STRUCT_PTR(node_parser_json, node_parser, string_dumper) = NULL;
    }
}

struct node_parser* node_parser_json_create(struct string_dumper* string_dumper) {
    MEM_ALLOC_STRUCT_DEF(node_parser_json);
    node_parser_json->parent.depth = 0;
    node_parser_json->parent.on_node_parser_start = on_node_parser_start_json;
    node_parser_json->parent.on_node_parser_stop = on_node_parser_stop_json;
    node_parser_json->parent.on_node_display = on_node_display_json;
    node_parser_json->parent.dispose = node_parser_json_dispose;
    node_parser_json->string_dumper = string_dumper;
    return MEM_STRUCT_AS(node_parser, node_parser_json);
}

