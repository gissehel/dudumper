#ifndef __COMMAND_LINE_PARSER_H
#define __COMMAND_LINE_PARSER_H

struct global_configuration;

struct global_configuration* command_line_parser_parse(int argc, char** argv);
void command_line_parser_get_help();

#endif /* __COMMAND_LINE_PARSER_H */
