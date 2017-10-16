#ifndef __NODE_PARSE_H
#define __NODE_PARSE_H

/*
 * #include "node_info.h"
 */
struct node_info;

struct node_parser_output {
    void (*on_node_parser_start)();
    void (*on_node_parser_stop)();
    void (*on_node_display)(struct node_info* node_info_item);
};

struct node_parser_data {
    struct node_info* node_info_root;
    struct node_info* node_info_last;
    const struct node_parser_output* output;
};

void node_parse(struct node_parser_output* node_parser_output, const char* path);
struct node_parser_output* node_parser_output_create();
struct node_parser_data* node_parser_data_create(const struct node_parser_output*);

#endif /* __NODE_PARSE_H */
