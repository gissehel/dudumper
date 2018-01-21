CC = gcc
LINK = gcc
CFLAGS = -Wall -Wextra
LFLAGS = -static
OBJS = main.o command_line_parser.o global_configuration.o node_info.o node_info_consumer.o node_info_consumer_html.o node_info_consumer_html_include.o node_info_consumer_json.o node_info_consumer_sqlite.o node_parser.o str_utils.o string_dumper.o string_dumper_file.o string_dumper_stdout.o mem_utils.o

all: dudumper

clean:
	rm -rf dudumper $(OBJS)

dudumper: $(OBJS)
	$(LINK) $(LFLAGS) -o dudumper $(OBJS)

command_line_parser.o: command_line_parser.c command_line_parser.h global_configuration.h mem_utils.h
	$(CC) $(CFLAGS) -o command_line_parser.o -c command_line_parser.c

global_configuration.o: global_configuration.c global_configuration.h mem_utils.h
	$(CC) $(CFLAGS) -o global_configuration.o -c global_configuration.c

main.o: main.c command_line_parser.h global_configuration.h node_info.h node_info_consumer.h node_info_consumer_html.h node_info_consumer_json.h node_info_consumer_sqlite.h node_parser.h str_utils.h string_dumper_file.h string_dumper_stdout.h
	$(CC) $(CFLAGS) -o main.o -c main.c

mem_utils.o: mem_utils.c mem_utils.h
	$(CC) $(CFLAGS) -o mem_utils.o -c mem_utils.c

node_info.o: node_info.c node_info.h mem_utils.h
	$(CC) $(CFLAGS) -o node_info.o -c node_info.c

node_info_consumer.o: node_info_consumer.c node_info_consumer.h mem_utils.h
	$(CC) $(CFLAGS) -o node_info_consumer.o -c node_info_consumer.c

node_info_consumer_html.o: node_info_consumer_html.c node_info_consumer_html.h node_info_consumer_html_include.h node_info_consumer_json.h mem_utils.h node_info.h node_info_consumer.h str_utils.h string_dumper.h
	$(CC) $(CFLAGS) -o node_info_consumer_html.o -c node_info_consumer_html.c

node_info_consumer_html_include.o: node_info_consumer_html_include.c node_info_consumer_html_include.h
	$(CC) $(CFLAGS) -o node_info_consumer_html_include.o  -c node_info_consumer_html_include.c

node_info_consumer_json.o: node_info_consumer_json.c node_info_consumer_json.h mem_utils.h node_info.h node_info_consumer.h str_utils.h string_dumper.h
	$(CC) $(CFLAGS) -o node_info_consumer_json.o -c node_info_consumer_json.c

node_info_consumer_sqlite.o: node_info_consumer_sqlite.c node_info_consumer_sqlite.h mem_utils.h node_info.h node_info_consumer.h str_utils.h string_dumper.h
	$(CC) $(CFLAGS) -o node_info_consumer_sqlite.o  -c node_info_consumer_sqlite.c

node_parser.o: node_parser.c node_parser.h node_info.h mem_utils.h
	$(CC) $(CFLAGS) -o node_parser.o -c node_parser.c

	$(CC) $(CFLAGS) -o str_utils.o -c str_utils.c

string_dumper.o: string_dumper.c string_dumper.h mem_utils.h
	$(CC) $(CFLAGS) -o string_dumper.o -c string_dumper.c

string_dumper_file.o: string_dumper_file.c string_dumper_file.h string_dumper.h str_utils.h mem_utils.h
	$(CC) $(CFLAGS) -o string_dumper_file.o -c string_dumper_file.c

string_dumper_stdout.o: string_dumper_stdout.c string_dumper_stdout.h string_dumper.h str_utils.h mem_utils.h
	$(CC) $(CFLAGS) -o string_dumper_stdout.o -c string_dumper_stdout.c

run: dudumper
	./dudumper --json --dir /usr/share --out z-usr-share

runp: dudumper
	./dudumper --json --dir .

g: main
	gdb ./dudumper .

