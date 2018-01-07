#include <stdio.h>
#include <stdlib.h>
#include "node_info.h"
#include "node_parser.h"
#include "string_dumper.h"
#include "node_parser_json.h"
#include "str_utils.h"

struct node_parser_json {
    struct node_parser parent;
    struct string_dumper* string_dumper;
};


void on_node_parser_start_json(const struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_json*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "[\n");
    }
}

void on_node_parser_stop_json(const struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_json*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->dump(string_dumper, "{}]\n");
    }
}

void on_node_display_json(const struct node_parser* node_parser, struct node_info* node_info_item) {
    char* name = doublequotestrdup_backquote(node_info_item->name);
    char* path = doublequotestrdup_backquote(node_info_item->path);
    struct string_dumper* string_dumper = ((struct node_parser_json*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->dump(
            string_dumper, 
            "{\"id\":%ld, \"name\":\"%s\", \"path\":\"%s\", \"depth\":%ld, \"parent_id\":%ld, \"size\":%ld, \"occ_size\":%ld, \"is_dir\":%s},\n",  
            node_info_item->id, 
            name, 
            path, 
            node_info_item->depth, 
            node_info_item->parent == NULL ? 0 : node_info_item->parent->id, node_info_item->size, 
            node_info_item->occ_size, 
            node_info_item->is_dir ? "true" : "false"
        );
    }
    free(name);
    free(path);
}

void node_parser_json_dispose(struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_json*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->close(string_dumper);
        ((struct node_parser_json*)node_parser)->string_dumper = NULL;
    }
}

struct node_parser* node_parser_json_create(struct string_dumper* string_dumper) {
    struct node_parser_json* node_parser = malloc(sizeof(struct node_parser_json));
    node_parser->parent.depth = 0;
    node_parser->parent.on_node_parser_start = on_node_parser_start_json;
    node_parser->parent.on_node_parser_stop = on_node_parser_stop_json;
    node_parser->parent.on_node_display = on_node_display_json;
    node_parser->parent.dispose = node_parser_json_dispose;
    node_parser->string_dumper = string_dumper;
    return (struct node_parser*)node_parser;
}

