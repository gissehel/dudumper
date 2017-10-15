main.o: main.c
	gcc -o main.o -c main.c

node_parser.o: node_parser.c node_parser.h node_info.h
	gcc -o node_parser.o -c node_parser.c

node_info.o: node_info.c node_info.h
	gcc -o node_info.o -c node_info.c

main: main.o node_info.o node_parser.o
	gcc -o main main.o node_info.o node_parser.o

run: main
	./main /usr/share

runp: main
	./main .


