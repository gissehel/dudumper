#ifndef __NODE_PARSER_H
#define __NODE_PARSER_H

struct node_info;
struct node_parser_data;

struct node_parser {
    void (*on_node_parser_start)(const struct node_parser* node_parser);
    void (*on_node_parser_stop)(const struct node_parser* node_parser);
    void (*on_node_display)(const struct node_parser* node_parser, struct node_info* node_info_item);
    void (*dispose)(struct node_parser* node_parser);
};


void node_parser_parse(struct node_parser* node_parser, const char* path);
struct node_parser_data* node_parser_data_create(const struct node_parser*);

#endif /* __NODE_PARSER_H */
