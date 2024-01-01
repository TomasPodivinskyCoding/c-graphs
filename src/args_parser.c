#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include "args_parser.h"
#include "ansii.h"

#define helpKey 400

bool parseArgumentNumber(int *to, char *str) {
    char *endptr = NULL;
    *to = strtol(str, &endptr, 10);

    if (endptr == str) {
        printError("Could not parse %s to a number\n", str);
    }
    return endptr == str;
}

StructuredArgs structuredArgsCtor() {
    StructuredArgs args;
    args.showHelp = 0;
    args.algorithmKey = -1;
    args.file = NULL;
    args.startValue = -1;
    args.endValue = -1;
    return args;
}

bool parseFileArg(StructuredArgs *args, int argc, char **argv) {
    if (argc < 2) {
        return false;
    }
    args->file = fopen(argv[1], "r");
    if (args->file == NULL) {
        fprintf(stderr, "Could not read file\n");
    }
    return args->file != NULL;
}

bool handleOption(int argumentCode, int argc, char **argv, StructuredArgs *args) {
    switch (argumentCode) {
        case helpKey:
            args->showHelp = true;
            break;
        case dijkstraKey:
        case bfsKey:
        case dfsKey:
            args->algorithmKey = argumentCode;
            if (!parseFileArg(args, argc, argv)) {
                return false;
            }
            if (optind + 1 >= argc) {
                // printUsage();
                return false;
            }
            parseArgumentNumber(&args->startValue, argv[optind]);
            parseArgumentNumber(&args->endValue, argv[optind + 1]);
            break;
        case '?': // TODO print usage
        default:
            return false;
    }
    return true;
}

bool parseArgs(int argc, char **argv, StructuredArgs *args) {
    struct option longOptions[] = {
            {"help", no_argument,  0, helpKey},
            {"dijkstra", no_argument,  0, dijkstraKey},
            {"bfs", no_argument, 0, bfsKey},
            {"dfs", no_argument, 0, dfsKey},
            {0, 0, 0, 0}
    };
    int optionIndex = 0;
    int argumentCode;

    while ((argumentCode = getopt_long(argc, argv, "", longOptions, &optionIndex)) != -1) {
        if (!handleOption(argumentCode, argc, argv, args)) {
            return false;
        }
    }

    return true;
}
