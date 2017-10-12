main.o: main.c
	gcc -o main.o -c main.c

main: main.o node_info.o
	gcc -o main main.o node_info.o

node_info.o: node_info.c node_info.h
	gcc -o node_info.o -c node_info.c

run: main
	./main /usr/share

runp: main
	./main .


