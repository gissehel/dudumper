#ifndef __NODE_PARSE_H
#define __NODE_PARSE_H

/*
 * #include "node_info.h"
 */
struct node_info;

struct node_parse_output {
    void (*on_node_parse_start)();
    void (*on_node_parse_stop)();
    void (*on_node_display)(struct node_info* node_info_item);
};

struct node_parse_data {
    struct node_info* node_info_root;
    struct node_info* node_info_last;
    const struct node_parse_output* output;
};

void node_parse(struct node_parse_data* node_parse_data, const char* path);
struct node_parse_output* node_parse_output_create();
struct node_parse_data* node_parse_data_create(const struct node_parse_output*);

#endif /* __NODE_PARSE_H */
