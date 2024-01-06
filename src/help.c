#include <stdio.h>
#include "ansii.h"

void printOptions() {
    printf("Options: \n");
    printf("  --dijkstra: runs Dijkstra's algorithm on given graph\n");
    printf("  --bfs:      runs BFS on given graph\n");
    printf("  --dfs:      runs DFS on given graph\n");
}

void printUsage(char *programName) {
    printf("Usage: \n");
    printf("  %s <file> (--dijkstra | --bfs | --dfs) <start_node> <end_node>\n", programName);
}

void printHelp(char *programName) {
    printf(BOLDWHITE "c-graphs\n" RESET);
    printf("A small tool to try out some of the main graph algorithms.\n");
    printf("\n");
    printUsage(programName);
    printf("\n");
    printOptions();
}
