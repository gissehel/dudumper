#ifndef __NODE_PARSER_H
#define __NODE_PARSER_H

struct node_info_consumer;
struct node_parser;

void node_parser_start(const struct node_parser* node_parser);
void node_parser_parse(struct node_parser* node_parser, const char* path, bool consider_dot_dir_as_file);
void node_parser_stop(const struct node_parser* node_parser);

struct node_parser* node_parser_create(struct node_info_consumer* node_info_consumer, long depth);
void node_parser_free(struct node_parser* node_parser);

#endif /* __NODE_PARSER_H */
