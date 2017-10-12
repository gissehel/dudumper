main.o: main.c
	gcc -o main.o -c main.c

main: main.o node_info.o node_path.o
	gcc -o main main.o node_info.o node_path.o

node_info.o: node_info.c node_info.h
	gcc -o node_info.o -c node_info.c

node_path.o: node_path.c node_path.h node_info.h
	gcc -o node_path.o -c node_path.c

run: main
	./main /usr/share

runp: main
	./main .


