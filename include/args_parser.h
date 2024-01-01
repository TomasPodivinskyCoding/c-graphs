#ifndef C_GRAPHS_ARGS_PARSER_H
#define C_GRAPHS_ARGS_PARSER_H

#include <stdio.h>
#include <stdbool.h>

#define dijkstraKey 300
#define bfsKey 301
#define dfsKey 302

typedef struct {
    int showHelp;
    int algorithmKey;
    FILE *file;
    int startValue;
    int endValue;
} StructuredArgs;

StructuredArgs structuredArgsCtor();
bool parseArgs(int argc, char **argv, StructuredArgs *args);

#endif //C_GRAPHS_ARGS_PARSER_H
