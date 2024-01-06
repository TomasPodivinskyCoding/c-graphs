#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include "args_parser.h"
#include "ansii.h"
#include "help.h"

#define helpKey 400

ProgramArgs programArgsCtor() {
    ProgramArgs args;
    args.showHelp = 0;
    args.algorithmKey = -1;
    args.file = NULL;
    args.startValue = -1;
    args.endValue = -1;
    return args;
}

bool parseArgumentNumber(int *to, char *str) {
    char *endptr = NULL;
    *to = strtol(str, &endptr, 10);

    // TODO I ain't sure if this covers all errors that can occur in strtol
    if (endptr == str) {
        printError("Could not parse %s to a number\n", str);
    }
    return endptr != str;
}

bool parseFileArg(ProgramArgs *args, int argc, char **argv) {
    if (argc < 2) {
        return false;
    }
    args->file = fopen(argv[1], "r");
    if (args->file == NULL) {
        fprintf(stderr, "Could not read file\n");
    }
    return args->file != NULL;
}

bool handleOption(int argumentCode, int argc, char **argv, ProgramArgs *args) {
    switch (argumentCode) {
        case dijkstraKey:
        case bfsKey:
        case dfsKey:
            args->algorithmKey = argumentCode;
            bool areArgsCorrect =
                    optind + 1 < argc
                    && parseFileArg(args, argc, argv)
                    && parseArgumentNumber(&args->startValue, argv[optind])
                    && parseArgumentNumber(&args->endValue, argv[optind + 1]);
            if (!areArgsCorrect) {
                printUsage(argv[0]);
                return false;
            }

            break;
        case '?':
        default:
            printUsage(argv[0]);
            return false;
    }
    return true;
}

bool parseArgs(int argc, char **argv, ProgramArgs *args) {
    struct option longOptions[] = {
            {"help", no_argument,  &args->showHelp, helpKey},
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
