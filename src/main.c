#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include "graphs.h"
#include "algorithms.h"
#include "ansii.h"
#include "adjacency_matrix_parser.h"
#include "args_parser.h"
#include "help.h"
#include "print_table.h"
#include "char_utils.h"

char ***prepareDijkstraOutputRows(Graph *graph, Node *start, int rowCount) {
    char ***rowValues = malloc(graph->size * sizeof(char*));

    int startValueDigitCount = digitCount(start->value);
    int maxPathLength = 0;
    for (int row = 0; row < rowCount; row++) {
        char **cols = malloc(3 * sizeof(char *));
        cols[0] = malloc((startValueDigitCount + digitCount(graph->nodes[row].value)) * sizeof(char) + 2 + 1);
        sprintf(cols[0], "%d->%d", start->value, graph->nodes[row].value);

        cols[1] = malloc(digitCount(graph->nodes[row].distance) * sizeof(char) + 1 + 2);
        sprintf(cols[1], "%d", graph->nodes[row].distance);

        String path = nodePath(graph->nodes[row]);
        cols[2] = path.value;
        if ((int) strlen(path.value) > maxPathLength) {
            maxPathLength = (int) strlen(path.value);
        }

        rowValues[row] = cols;
    }
    return rowValues;
}

void printDijkstraOutput(Graph *graph, Node *start) {
    // TODO assert and free do pici to bude bolest
    TableParams tableParams;
    tableParams.colCount = 3;
    tableParams.rowCount = graph->size;

    char *headerValues[] = { "Vertex", "Distance", "Path" };
    tableParams.headerValues = headerValues;

    tableParams.rowValues = prepareDijkstraOutputRows(graph, start, tableParams.rowCount);
    printf("\n");
    printTable(&tableParams);
    freeTable(&tableParams);
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

    if (args.algorithmKey == dijkstraKey) {
        dijkstra(start, end, graph.size);
        printDijkstraOutput(&graph, start);
        graphDtor(&graph);
        return 0;
    }

	Node *lastNode = NULL;
    switch (args.algorithmKey) {
        case bfsKey:
            lastNode = bfs(start, end, graph.size);
            break;
        case dfsKey:
            lastNode = dfs(start, end, graph.size);
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

