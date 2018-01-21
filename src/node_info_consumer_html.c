#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mem_utils.h"
#include "node_info.h"
#include "node_info_consumer.h"
#include "node_info_consumer_html.h"
#include "node_info_consumer_html_include.h"
#include "node_info_consumer_json.h"
#include "str_utils.h"
#include "string_dumper.h"

struct node_info_consumer_html {
    struct node_info_consumer parent;
    struct node_info_consumer* node_info_consumer_json;
    struct string_dumper* string_dumper;
};

void node_info_consumer_html_start(const struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, string_dumper, string_dumper);
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, node_info_consumer_json, node_info_consumer);
    if (string_dumper != NULL && node_info_consumer_json != NULL) {
        STRING_DUMPER_DUMP(string_dumper, "%s", node_info_consumer_html_prefix);
        NODE_INFO_CONSUMER_START(node_info_consumer_json);
    }
}

void node_info_consumer_html_stop(const struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, string_dumper, string_dumper);
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, node_info_consumer_json, node_info_consumer);
    if (string_dumper != NULL && node_info_consumer_json != NULL) {
        NODE_INFO_CONSUMER_STOP(node_info_consumer_json);
        STRING_DUMPER_DUMP(string_dumper, "%s", node_info_consumer_html_suffix);
    }
}

void node_info_consumer_html_consume_node(const struct node_info_consumer* node_info_consumer, struct node_info* node_info) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, node_info_consumer_json, node_info_consumer);
    if (node_info_consumer_json != NULL) {
        NODE_INFO_CONSUMER_CONSUME_NODE(node_info_consumer_json, node_info);
    }
}

void node_info_consumer_html_dispose(struct node_info_consumer* node_info_consumer) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, string_dumper, string_dumper);
    MEM_STRUCT_PTR_DEF_STRUCT(node_info_consumer_html, node_info_consumer, node_info_consumer_json, node_info_consumer);
    if (string_dumper != NULL) {
        STRING_DUMPER_CLOSE(string_dumper);
        MEM_STRUCT_PTR(node_info_consumer_html, node_info_consumer, string_dumper) = NULL;
    }
    if (node_info_consumer_json != NULL) {
        node_info_consumer_free(node_info_consumer_json);
        MEM_STRUCT_PTR(node_info_consumer_html, node_info_consumer, node_info_consumer_json) = NULL;
    }
}

struct node_info_consumer* node_info_consumer_html_create(struct string_dumper* string_dumper) {
    MEM_ALLOC_STRUCT_DEF(node_info_consumer_html);
    node_info_consumer_html->parent.on_start = node_info_consumer_html_start;
    node_info_consumer_html->parent.on_stop = node_info_consumer_html_stop;
    node_info_consumer_html->parent.on_consume_node = node_info_consumer_html_consume_node;
    node_info_consumer_html->parent.on_dispose = node_info_consumer_html_dispose;
    node_info_consumer_html->node_info_consumer_json = node_info_consumer_json_create(string_dumper);
    node_info_consumer_html->string_dumper = string_dumper;
    return MEM_STRUCT_AS(node_info_consumer, node_info_consumer_html);
}

