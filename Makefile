CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Werror -D DEBUG

main: main.c algorithms.c graphs.c

run: main
	./main ./adjacencyMatrix
