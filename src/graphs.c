#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include "graphs.h"
#include "ansii.h"
#include "math_utils.h"
#include "string_builder.h"

Node nodeCtor(int value) {
	Node node;
	node.edges = NULL;
	node.value = value;
	node.distance = INT_MAX;
	node.edgeCount = 0;
	node.status = NOT_VISITED;
	node.parent = NULL;
	return node;
}

void nodeAddEdge(Node *node, int distance, Node *to) {
	Edge edge;
	edge.distance = distance;
	edge.node = to;

	node->edgeCount = node->edgeCount + 1;

	Edge *edges = realloc(node->edges, node->edgeCount * sizeof(Edge));
	assert(edges != NULL && "Could not alloce enough memory for graph\n");
	edges[node->edgeCount - 1] = edge;

	node->edges = edges;
}

void nodeToInitialState(Node *node) {
	node->status = NOT_VISITED;
	node->parent = NULL;
}

void nodePrint(Node *node) {
	printf("%d: ", node->value);
	for (int i = 0; i < node->edgeCount; i++) {
		printf("%d,%d ", node->edges[i].node->value, node->edges[i].distance);
	}
	printf("\n");
}

void nodeBacktrackPrint(Node *node) {
	if (node->parent != NULL) {
		nodeBacktrackPrint(node->parent);
        printf(" %d", node->value);
	} else {
        printf("%d", node->value);
    }
}

int nodeParentCount(Node node) {
    int parentCount = 0;
    while (node.parent != NULL) {
        parentCount++;
        node.parent = node.parent->parent;
    }
    return parentCount;
}

String nodePath(Node node) {
    String path = stringCtor();
    Node *pNode = &node;
    while (true) {
        stringAppendInt(&path, pNode->value);
        pNode = pNode->parent;
        if (pNode != NULL) {
            stringAppendChar(&path, '>');
            stringAppendChar(&path, '-');
        } else {
            break;
        }
    }
    stringReverse(&path);
    return path;
}

Graph graphCtor(int size, Node *nodes) {
	Graph graph;
	graph.size = size;
	graph.nodes = nodes;
	return graph;
}

Graph emptyGraph() {
	Graph graph;
	graph.size = 0;
	graph.nodes = NULL;
	return graph;
}

bool appendNode(Graph *graph, Node node) {
	graph->size++;
	Node *nodes = realloc(graph->nodes, graph->size * sizeof(Node));
    if (nodes == NULL) {
        printf("Could not allocate enough memory.\n");
        return false;
    }
	nodes[graph->size - 1] = node;
	graph->nodes = nodes;
    return true;
}

void graphDtor(Graph *graph) {
	if (graph == NULL) {
		return;
	}
	for (int i = 0; i < graph->size; i++) {
		free(graph->nodes[i].edges);
	}
	free(graph->nodes);
	graph->nodes = NULL;
	graph->size = 0;
}

void printGraphValues(Graph *graph) {
	for (int i = 0; i < graph->size; i++) {
		printf("%d ", graph->nodes[i].value);
	}
}

void printGraphValidValuesError(Graph *graph) {
	printf("Valid values are: ");
	printGraphValues(graph);
	printf("\n");
}

Node *getGraphNode(Graph *graph, int value) {
	for (int i = 0; i < graph->size; i++) {
		if (graph->nodes[i].value == value) {
			return &graph->nodes[i];
		}
	}
	return NULL;
}

