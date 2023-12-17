#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphs.h"
#include "algorithms.h"

#ifdef DEBUG
#define dprintf(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define dprintf(fmt, ...) do {} while (0)
#endif

bool isNumber(char c) {
	return c >= '0' && c <= '9';
}

/** Use this function to move pointer to the next digit character or '\0' */
void movePointerToNextDigit(char **line) {
	while (!isNumber(**line) && **line != '\0') {
		(*line)++;
	}
}

Graph createGraphFromHeaderValues(int headerValues[], int valuesCount) {
	Node **nodes = malloc(valuesCount * sizeof(Node*));
	for (int i = 0; i < valuesCount; i++) {
		nodes[i] = nodeCtor(headerValues[i]); // toto by mozna slo i bez malloc
	}
	return graphCtor(valuesCount, nodes);
}

bool parseAdjacencyMatrixHeader(char *line, int **headerValuesOut, int *size) {
	int valuesCount = 0;
	int *headerValues = NULL;

	char *endPtr;
	while (*line != '\0') {
		// TODO
		int value = strtol(line, &endPtr, 10);
		dprintf("%d ", value);
		valuesCount++;
		int *newHeaderValues = realloc(headerValues, valuesCount * sizeof(int));
		if (newHeaderValues == NULL) {
			*size = 0;
			return false;
		}
		headerValues = newHeaderValues;
		headerValues[valuesCount - 1] = value;

		line = endPtr;
		movePointerToNextDigit(&line);
	}

	*size = valuesCount;
	*headerValuesOut = headerValues;
	return true;
}

Node *getGraphNode(Graph *graph, int value) {
	for (int i = 0; i < graph->size; i++) {
		if (graph->nodes[i]->value == value) {
			return graph->nodes[i];
		}
	}
	return NULL;
}

// Graph loadGraphFromAdjacencyMatrix(FILE *file) {
// 	char *line = NULL;
// 	size_t cap = 0;
// 	if (getline(&line, &cap, file) == EOF) {
// 		free(line);
// 		return emptyGraph();
// 	}
// 	
// 	int *headerValues;
// 	int valuesCount;
// 	parseAdjacencyMatrixHeader(line, &headerValues, &valuesCount);
// 	dprintf("\nValues count: %d\n", valuesCount);
// 	if (headerValues == NULL) {
// 		free(line);
// 		free(headerValues);
// 		return emptyGraph();
// 	}
// 
// 	Graph graph = createGraphFromHeaderValues(headerValues, valuesCount);
// 	dprintf("%s", "Graph created\n");
// 	char *endPtr;
// 	while (getline(&line, &cap, file) != EOF) {
// 		char *line2 = line;
// 		// TODO error handling
// 		int nodeValue = strtol(line2, &endPtr, 10); 			
// 		Node *currentNode = getGraphNode(&graph, nodeValue);
// 		if (currentNode == NULL) {
// 			// TODO, it should be easy to add a very descriptive error message here!
// 			free(line);
// 			free(headerValues);
// 			graphDtor(&graph);
// 			return emptyGraph();
// 		}
// 		line2 = endPtr;
// 		movePointerToNextDigit(&line2);
// 		int index = 0;
// 		while (*line2 != '\0') {
// 			int distance = strtol(line2, &endPtr, 10); 			
// 			nodeAddEdgeBothDirections(currentNode, graph.nodes[index], distance);
// 			dprintf(" %d", distance);
// 			line2 = endPtr;
// 			movePointerToNextDigit(&line2);
// 			index++;
// 		}
// 		dprintf("%s", "\n");
// 	}
// 
// 	free(headerValues);
// 	free(line);
// 	return graph;
// }

bool parseRow(Graph *graph, char *row) {
	// TODO error handling
	char *endPtr;
	int nodeValue = strtol(row, &endPtr, 10); 			
	Node *currentNode = getGraphNode(graph, nodeValue);
	if (currentNode == NULL) {
		// TODO, it should be easy to add a very descriptive error message here!
		free(row);
		free(headerValues);
		graphDtor(&graph);
		return false;
	}
	row = endPtr;
	movePointerToNextDigit(&row);
	int index = 0;
	while (*row != '\0') {
		int distance = strtol(row, &endPtr, 10); 			
		nodeAddEdgeBothDirections(currentNode, graph.nodes[index], distance);
		dprintf(" %d", distance);
		row = endPtr;
		movePointerToNextDigit(&row);
		index++;
	}
	dprintf("%s", "\n");
	return true;
}

Graph loadGraphFromAdjacencyMatrix(FILE *file) {
	char *line = NULL;
	size_t cap = 0;
	if (getline(&line, &cap, file) != EOF) {
		int *headerValues;
		int valuesCount;
		parseAdjacencyMatrixHeader(line, &headerValues, &valuesCount);
		dprintf("\nValues count: %d\n", valuesCount);
		if (headerValues != NULL) {
			Graph graph = createGraphFromHeaderValues(headerValues, valuesCount);
			dprintf("%s", "Graph created\n");
			char *endPtr;
			while (getline(&line, &cap, file) != EOF) {
				if (!parseRow(&graph, line)) {
					return emptyGraph();
				}
			}
			free(headerValues);
		}
	}
	free(line);

	return graph;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Wrong number of arguments\n");
		return 1;
	}
	char *filename = argv[1];
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not read file\n");
		return 1;
	}
	Graph graph = loadGraphFromAdjacencyMatrix(file);
	fclose(file);
	if (graph.nodes != NULL) {
		graphDtor(&graph);
	}

	return 0;
}

