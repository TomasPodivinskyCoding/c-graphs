#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include "graphs.h"
#include "algorithms.h"
#include "ansii.h"
#include "adjacency_matrix_parser.h"
#include "args_parser.h"
#include "help.h"
#include "print_table.h"
#include "char_utils.h"

#define TERMINATOR_LENGTH 1

void setVertexText(char **to, Graph *graph, int startValueDigitCount, int row, int startValue) {
    char *separator = "->";
    int nodeValueDigitCount = digitCount(graph->nodes[row].value);
    int stringLength = startValueDigitCount + nodeValueDigitCount + (int) strlen(separator) + TERMINATOR_LENGTH;
    int stringSize = (int) (stringLength * sizeof(char));
    *to = malloc(stringSize);
    sprintf(*to, "%d%s%d", startValue, separator, graph->nodes[row].value);
}

void setDistanceText(char **to, Graph *graph, int row) {
    if (graph->nodes[row].distance == INT_MAX) {
        char *infinity = "Infinity";
        int stringLength = (int) strlen(infinity) + TERMINATOR_LENGTH;
        int stringSize = (int) (stringLength * sizeof(char));
        *to = malloc(stringSize);
        memcpy(*to, infinity, stringSize);
    } else {
        int stringSize = (int) ((digitCount(graph->nodes[row].distance) + TERMINATOR_LENGTH) * sizeof(char));
        *to = malloc(stringSize);
        sprintf(*to, "%d", graph->nodes[row].distance);
    }
}

void setPathText(char **to, Graph *graph, int row, int *maxPathLength) {
    String path = nodePath(graph->nodes[row]);
    *to = path.value;
    if ((int) strlen(path.value) > *maxPathLength) {
        *maxPathLength = (int) strlen(path.value);
    }
}

char ***prepareDijkstraOutputRows(Graph *graph, Node *start, int rowCount) {
    char ***rowValues = malloc(graph->size * sizeof(char*));

    int startValueDigitCount = digitCount(start->value);
    int maxPathLength = 0;
    for (int row = 0; row < rowCount; row++) {
        char **cols = malloc(3 * sizeof(char *));
        setVertexText(&cols[0], graph, startValueDigitCount, row, start->value);
        setDistanceText(&cols[1], graph, row);
        setPathText(&cols[2], graph, row, &maxPathLength);

        rowValues[row] = cols;
    }
    return rowValues;
}

void printDijkstraOutput(Graph *graph, Node *start) {
    char *headerValues[] = { "Vertex", "Distance", "Path" };
    TableParams tableParams = tableParamsCtor(
            headerValues,
            3,
            graph->size,
            prepareDijkstraOutputRows(graph, start, graph->size)
    );

    printTable(&tableParams);
    freeTableParams(&tableParams);
}

int main(int argc, char **argv) {
	ProgramArgs args = programArgsCtor();
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
    bool isNotDijkstra = args.algorithmKey != dijkstraKey;
    switch (args.algorithmKey) {
        case dijkstraKey:
            dijkstra(start, end, graph.size);
            printDijkstraOutput(&graph, start);
            break;
        case bfsKey:
            lastNode = bfs(start, end, graph.size);
            break;
        case dfsKey:
            lastNode = dfs(start, end, graph.size);
            break;
        default:
            fprintf(stderr, "Something went wrong...");
            break;
    }

    if (isNotDijkstra) {
        if (lastNode == NULL) {
            fprintf(stderr, "Could not reach %d from %d\n", args.endValue, args.startValue);
        } else {
            nodeBacktrackPrint(lastNode);
            printf("\n");
        }
    }
	graphDtor(&graph);

	return 0;
}

