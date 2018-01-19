#ifndef __NODE_DUMPER_H
#define __NODE_DUMPER_H

struct node_info;

struct node_dumper {
    struct node_dumper* node_dumper;
    struct string_dumper* string_dumper;
};

struct node_dumper* node_dumper_create( );

void node_parse(struct node_parser_output* node_parser_output, const char* path);
struct node_parser_output* node_parser_output_create();

#endif /* __NODE_DUMPER_H */
