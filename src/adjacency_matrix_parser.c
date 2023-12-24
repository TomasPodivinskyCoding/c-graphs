#define  _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ansii.h"
#include "graphs.h"

bool isNumber(char c) {
	return c >= '0' && c <= '9';
}

/** Use this function to move pointer to the next digit character or '\0' */
void movePointerToNextDigit(char **line) {
	while (!isNumber(**line) && **line != '\0') {
		(*line)++;
	}
}

bool parseAdjacencyMatrixHeader(Graph *graph, char *line) { 
	while (*line != '\0') {
		// TODO error handling?
		char *endPtr;
		int value = strtol(line, &endPtr, 10);
		Node node = nodeCtor(value);
		appendNode(graph, &node);

		line = endPtr;
		movePointerToNextDigit(&line);
	}

	return true;
}

void handleMissingRowValues(FILE *file, int errorIndex) {
	printError("%s", "Encountered missing values in your input matrix.\n");

	errno = 0;
	rewind(file);
	if (errno != 0) {
		return;
	}
	char *line = NULL;
	size_t cap = 0;

	int rowIndex = 0;
	while (getline(&line, &cap, file) != EOF) {
		if (rowIndex == errorIndex) {
			printf(BOLDRED "%s" RESET, line);
		} else {
			printf("%s", line);
		}
		rowIndex++;
	}
	free(line);
}

int parseRow(Graph *graph, char *row, Node *currentNode) {
	char *endPtr = NULL;
	int count = 0;
	while (*row != '\0') {
		int distance = strtol(row, &endPtr, 10); 			
		if (distance != 0) {
			nodeAddEdge(currentNode, distance, &graph->nodes[count]);
		}
		row = endPtr;
		movePointerToNextDigit(&row);
		count++;
	}
	return count;
}

void handleWrongFirstRowNode(Graph *graph, int nodeValue, char *row, int errorIndex, FILE *file) {
	printError("Could not find a node with value: %d\n", nodeValue);
	printGraphValidValuesError(graph);

	errno = 0;
	rewind(file);
	if (errno != 0) {
		return;
	}

	int rowIndex = 0;
	char *line = NULL;
	size_t cap = 0;
	while (getline(&line, &cap, file) != EOF) {
		if (rowIndex == errorIndex) {
			printf(BOLDRED "%d " RESET, nodeValue);
			printf("%s", row);
		} else {
			printf("%s", line);
		}
		rowIndex++;
	}
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

// TODO začít kontrolovat duplikátní hodnoty v matrixu
bool parseRows(FILE *file, Graph *graph, char *line, size_t cap) {
	int rowIndex = 1; // header is inluded
	while (getline(&line, &cap, file) != EOF) {
		char *linePointer = line;
		Node *rowHeader = parseRowHeader(graph, &linePointer, file, rowIndex);
		if (rowHeader == NULL) {
			return false;
		}
		int count = parseRow(graph, linePointer, rowHeader);
		if (count != graph->size) {
			handleMissingRowValues(file, rowIndex);
			return false;
		}
		rowIndex++;
	}
	return true;
}

bool parseAdjacencyMatrix(FILE *file, Graph *graph) {
	char *line = NULL;
	size_t cap = 0;
	bool wasParseSuccess = getline(&line, &cap, file) != EOF && parseAdjacencyMatrixHeader(graph, line) && graph->nodes != NULL && parseRows(file, graph, line, cap);
	free(line);
	if (!wasParseSuccess) {
		graphDtor(graph);
	}

	return wasParseSuccess;
}

