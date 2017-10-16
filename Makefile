main.o: main.c str_utils.h node_parser.h node_info.h
	gcc -o main.o -c main.c

node_parser.o: node_parser.c node_parser.h node_info.h
	gcc -o node_parser.o -c node_parser.c

node_info.o: node_info.c node_info.h
	gcc -o node_info.o -c node_info.c

str_utils.o: str_utils.c str_utils.h
	gcc -o str_utils.o -c str_utils.c
	
main: main.o node_info.o node_parser.o str_utils.o
	gcc -o main main.o node_info.o node_parser.o str_utils.o

clean:
	rm -rf main main.o node_info.o node_parser.o str_utils.o

run: main
	./main /usr/share

runp: main
	./main .


