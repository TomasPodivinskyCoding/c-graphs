#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graphs.h"
#include "algorithms.h"
#include "ansii.h"
#include "adjacency_matrix_parser.h"
#include "args_parser.h"

void printOptions() {
    // TODO it would be useful to have the program name here
    printf("Options: \n");
    printf("  --dijkstra: runs Dijkstra's algorithm on given graph\n");
    printf("  --bfs:      runs BFS on given graph\n");
    printf("  --dfs:      runs DFS on given graph\n");
}

void printUsage(char *programName) {
    // TODO it would be useful to have the program name here
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

int main(int argc, char **argv) {
	StructuredArgs args = structuredArgsCtor();
	if (!parseArgs(argc, argv, &args)) {
		return 1;
	}

    if (args.showHelp) {
        printHelp(argv[0]);
        return 0;
    }

	Graph graph = emptyGraph();
	bool wasParseSuccessful = parseAdjacencyMatrix(args.file, &graph);
	fclose(args.file);
	if (!wasParseSuccessful) {
		graphDtor(&graph);
		return 1;
	}

	Node *start = getGraphNode(&graph, args.startValue);
	Node *end = getGraphNode(&graph, args.endValue);
	if (start == NULL) {
		printError("%d is not a valid place to start the algorithm. Choose one of the values you from the graph.\n", args.startValue);
	}
	if (end == NULL) {
		printError("%d is not a valid place to end the algorithm. Choose one of the values you from the graph.\n", args.endValue);
	}
	if (start == NULL || end == NULL) {
		printGraphValidValuesError(&graph);
		graphDtor(&graph);
		return 1;
	}

	Node *lastNode = NULL;
    switch (args.algorithmKey) {
        case bfsKey:
            lastNode = bfs(start, end, graph.size);
            break;
        case dfsKey:
            lastNode = dfs(start, end, graph.size);
            break;
        case dijkstraKey:
            lastNode = dijkstra(start, end, graph.size);
            printf("Výsledná délka: %d\n", lastNode->distance);
            Node *h = lastNode;
            while (h != NULL) {
                printf("%d %d\n", h->value, h->distance);
                h = h->parent;
            }
            break;
        default:
            // TODO
            break;
    }

	if (lastNode == NULL) {
		fprintf(stderr, "Could not reach %d from %d\n", args.endValue, args.startValue);
	} else {
		printf("Výsledná trasa: ");
		nodeBacktrackPrint(lastNode);
		printf("\n");
	}
	graphDtor(&graph);

	return 0;
}

