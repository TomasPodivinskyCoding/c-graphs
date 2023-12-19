#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "graphs.h"
#include "algorithms.h"
#include "ansii.h"
#include "list.h"

#ifdef DEBUG
#define dprintf(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define dprintf(fmt, ...) do {} while (0)
#endif

typedef struct {
	char *filename;
	char *algorithm;
	int startValue;
	int endValue;
} StructuredArgs;

bool isNumber(char c) {
	return c >= '0' && c <= '9';
}

/** Use this function to move pointer to the next digit character or '\0' */
void movePointerToNextDigit(char **line) {
	while (!isNumber(**line) && **line != '\0') {
		(*line)++;
	}
}

bool parseAdjacencyMatrixHeader(char *line, List *header) {
	while (*line != '\0') {
		// TODO error handling
		char *endPtr;
		int value = strtol(line, &endPtr, 10);
		if (!listAppend(header, value)) {
			return false;
		}

		line = endPtr;
		movePointerToNextDigit(&line);
	}

	return true;
}

Node *getGraphNode(Graph *graph, int value) {
	for (int i = 0; i < graph->size; i++) {
		if (graph->nodes[i].value == value) {
			return &graph->nodes[i];
		}
	}
	return NULL;
}

bool parseRow(Graph *graph, char *row, Node *currentNode) {
	char *endPtr = NULL;
	int index = 0;
	while (*row != '\0') {
		int distance = strtol(row, &endPtr, 10); 			
		if (distance != 0 && !nodeAddEdgeBothDirections(currentNode, &graph->nodes[index], distance)) {
			return false;
		}
		row = endPtr;
		movePointerToNextDigit(&row);
		index++;
	}
	return true;
}

void handleWrongFirstRowNode(Graph *graph, int nodeValue, char *row, int errorIndex, FILE *file) {
	printError("Could not find a node with value: %d\n\n", nodeValue);
	char *line = NULL;
	size_t cap = 0;

	errno = 0;
	rewind(file);
	if (errno == 0) {
		int rowIndex = 0;
		while (getline(&line, &cap, file) != EOF) {
			if (rowIndex == errorIndex) {
				printf(BOLDRED "%d " RESET, nodeValue);
				printf("%s", row);
			} else {
				printf("%s", line);
			}
			rowIndex++;
		}
	}
	printf("\n");
	printGraphValidValuesError(graph);
	free(line);
}

Node *parseRowHeader(Graph *graph, char **row, FILE *file, int rowIndex) {
	char *endPtr = NULL;
	// TODO error handling
	int nodeValue = strtol(*row, &endPtr, 10); 			
	Node *currentNode = getGraphNode(graph, nodeValue);

	*row = endPtr;
	movePointerToNextDigit(row);
	if (currentNode == NULL) {
		handleWrongFirstRowNode(graph, nodeValue, *row, rowIndex, file);
	}
	return currentNode;
}

// TODO začít kontrolovat, jestli je dostatečný počet hodnot v jednotlivých řádcích
// TODO začít kontrolovat duplikátní hodnoty v matrixu
Graph loadGraphFromAdjacencyMatrix(FILE *file) {
	char *line = NULL;
	size_t cap = 0;
	Graph graph = emptyGraph();
	List headerValues = listCtor();

	if (getline(&line, &cap, file) != EOF) {
		if (parseAdjacencyMatrixHeader(line, &headerValues)) {
			graph = createGraphFromHeaderValues(&headerValues);
			int rowIndex = 1; // header is also a row
			while (getline(&line, &cap, file) != EOF) {
				char *linePointer = line;
				Node *rowHeader = parseRowHeader(&graph, &linePointer, file, rowIndex);
				if (rowHeader == NULL || !parseRow(&graph, linePointer, rowHeader)) {
					graphDtor(&graph);
					break;
				}
				rowIndex++;
			}
			listDtor(&headerValues);
		}
	}
	free(line);

	return graph;
}

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
	if (strcmp(args->algorithm, "--bfs") != 0 && strcmp(args->algorithm, "--dfs") != 0) {
		fprintf(stderr, "%s is not a valid algorithm name.\n", argv[1]);
		fprintf(stderr, "Valid algorithm names are: --bfs, --dfs");
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

	Graph graph = loadGraphFromAdjacencyMatrix(file);
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
	}
	if (lastNode == NULL) {
		fprintf(stderr, "Could not reach %d from %d\n", args.endValue, args.startValue);
	} else {
		printf("Výsledná trasa dfs: ");
		nodeBacktrackPrint(lastNode);
		printf("\n");
	}
	graphDtor(&graph);

	return 0;
}

