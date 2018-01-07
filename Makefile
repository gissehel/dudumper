main.o: main.c node_info.h node_parser.h node_parser_json.h node_parser_sqlite.h str_utils.h string_dumper_file.h string_dumper_stdout.h
	gcc -o main.o -c main.c

node_info.o: node_info.c node_info.h
	gcc -o node_info.o -c node_info.c

node_parser.o: node_parser.c node_parser.h node_info.h
	gcc -o node_parser.o -c node_parser.c

node_parser_json.o: node_parser_json.c node_parser_json.h node_parser.h node_info.h string_dumper.h str_utils.h
	gcc -o node_parser_json.o -c node_parser_json.c

node_parser_sqlite.o: node_parser_sqlite.c node_parser_sqlite.h node_parser.h node_info.h string_dumper.h str_utils.h
	gcc -o node_parser_sqlite.o -c node_parser_sqlite.c

str_utils.o: str_utils.c str_utils.h
	gcc -o str_utils.o -c str_utils.c

string_dumper_file.o: string_dumper_file.c string_dumper_file.h string_dumper.h str_utils.h
	gcc -o string_dumper_file.o -c string_dumper_file.c

string_dumper_stdout.o: string_dumper_stdout.c string_dumper_stdout.h string_dumper.h str_utils.h
	gcc -o string_dumper_stdout.o -c string_dumper_stdout.c

main: main.o node_info.o node_parser.o node_parser_json.o node_parser_sqlite.o str_utils.o string_dumper_file.o string_dumper_stdout.o
	gcc -o main main.o node_info.o node_parser.o node_parser_json.o node_parser_sqlite.o str_utils.o string_dumper_file.o string_dumper_stdout.o

clean:
	rm -rf main main.o node_info.o node_parser.o node_parser_json.o node_parser_sqlite.o str_utils.o string_dumper_file.o string_dumper_stdout.o


run: main
	./main /usr/share

runp: main
	./main .

g: main
	gdb ./main .

