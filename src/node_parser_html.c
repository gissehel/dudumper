#include <stdio.h>
#include <stdlib.h>
#include "node_parser.h"
#include "string_dumper.h"
#include "node_parser_html.h"
#include "node_parser_html_include.h"
#include "node_parser_json.h"
#include "mem_utils.h"

struct node_parser_html {
    struct node_parser parent;
    struct node_parser* node_parser_json;
    struct string_dumper* string_dumper;
};


void on_node_parser_start_html(const struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_html*)node_parser)->string_dumper;
    struct  node_parser* node_parser_json = ((struct node_parser_html*)node_parser)->node_parser_json;
    if (string_dumper != NULL && node_parser_json != NULL) {
        string_dumper->dump(string_dumper, "%s", node_parser_html_prefix);
        node_parser_json->on_node_parser_start(node_parser_json);
    }
}

void on_node_parser_stop_html(const struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_html*)node_parser)->string_dumper;
    struct  node_parser* node_parser_json = ((struct node_parser_html*)node_parser)->node_parser_json;
    if (string_dumper != NULL && node_parser_json != NULL) {
        node_parser_json->on_node_parser_stop(node_parser_json);
        string_dumper->dump(string_dumper, "%s", node_parser_html_suffix);
    }
}

void on_node_display_html(const struct node_parser* node_parser, struct node_info* node_info_item) {
    struct  node_parser* node_parser_json = ((struct node_parser_html*)node_parser)->node_parser_json;
    if (node_parser_json != NULL) {
        node_parser_json->on_node_display(node_parser_json, node_info_item);
    }
}

void node_parser_html_dispose(struct node_parser* node_parser) {
    struct string_dumper* string_dumper = ((struct node_parser_html*)node_parser)->string_dumper;
    if (string_dumper != NULL) {
        string_dumper->close(string_dumper);
        ((struct node_parser_html*)node_parser)->string_dumper = NULL;
    }
    struct  node_parser* node_parser_json = ((struct node_parser_html*)node_parser)->node_parser_json;
    if (node_parser_json != NULL) {
        node_parser_json->dispose(node_parser_json);
        ((struct node_parser_html*)node_parser)->node_parser_json = NULL;
    }
}

struct node_parser* node_parser_html_create(struct string_dumper* string_dumper) {
    struct node_parser_html* node_parser = mem_alloc(sizeof(struct node_parser_html));
    struct node_parser* node_parser_json = node_parser_json_create(string_dumper);
    node_parser->parent.depth = 0;
    node_parser->parent.on_node_parser_start = on_node_parser_start_html;
    node_parser->parent.on_node_parser_stop = on_node_parser_stop_html;
    node_parser->parent.on_node_display = on_node_display_html;
    node_parser->parent.dispose = node_parser_html_dispose;
    node_parser->node_parser_json = node_parser_json;
    node_parser->string_dumper = string_dumper;
    return (struct node_parser*)node_parser;
}

