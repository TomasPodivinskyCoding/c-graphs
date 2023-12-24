#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graphs.h"
#include "algorithms.h"
#include "ansii.h"
#include "adjacency_matrix_parser.h"
#include "priority_queue.h"

typedef struct {
	char *filename;
	char *algorithm;
	int startValue;
	int endValue;
} StructuredArgs;

bool parseArgumentNumber(int *to, char *str) {
	char *endptr = NULL;
	*to = strtol(str, &endptr, 10);

	if (endptr == str) {
		printError("Could not parse %s to a number\n", str);
		return false;
	}
	return true;
}

bool parseArgs(int argc, char **argv, StructuredArgs *args) {
	if (argc < 4) {
		fprintf(stderr, "You have supplied a wrong number of arguments.\n");
		fprintf(stderr, "Usage: ./main [algorithm name] [file containing an adjacency matrix] [value of start node] [value of end node]");
		return false;
	}
	args->algorithm = argv[1];
	if (strcmp(args->algorithm, "--bfs") != 0 && strcmp(args->algorithm, "--dfs") != 0 && strcmp(args->algorithm, "--dijkstra")) {
		fprintf(stderr, "%s is not a valid algorithm name.\n", argv[1]);
		fprintf(stderr, "Valid algorithm names are: --bfs, --dfs, --dijkstra\n");
		return false;
	}
	args->filename = argv[2];
	
	return parseArgumentNumber(&args->startValue, argv[3]) && parseArgumentNumber(&args->endValue, argv[4]);
}

int main(int argc, char **argv) {
	StructuredArgs args;
	if (!parseArgs(argc, argv, &args)) {
		return 1;
	}
	FILE *file = fopen(args.filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not read file\n");
		return 1;
	}

	Graph graph = emptyGraph();
	parseAdjacencyMatrix(file, &graph);
	fclose(file);
	if (graph.nodes == NULL || graph.size == 0) {
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
	if (strcmp(args.algorithm, "--bfs") == 0) {
		lastNode = bfs(start, end, graph.size);
	} else if (strcmp(args.algorithm, "--dfs") == 0) {
		lastNode = dfs(start, end, graph.size);
	} else if (strcmp(args.algorithm, "--dijkstra") == 0) {
		lastNode = dijkstra(start, end, graph.size);
		printf("Výsledná délka: %d\n", lastNode->distance);
		Node *h = lastNode;
		while (h != NULL) {
			printf("%d %d\n", h->value, h->distance);
			h = h->parent;
		}
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

