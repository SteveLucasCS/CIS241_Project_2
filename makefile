main: main.o
	gcc -Wall main.o -o main
main.o: src/main.c