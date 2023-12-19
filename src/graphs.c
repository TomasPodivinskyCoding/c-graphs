#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphs.h"
#include "ansii.h"

Node nodeCtor(int value) {
	Node node;
	node.edges = NULL;
	node.value = value;
	node.edgeCount = 0;
	node.status = NOT_VISITED;
	node.parent = NULL;
	return node;
}

bool nodeAddEdge(Node *node, int distance, Node *to) {
	Edge edge;
	edge.distance = distance;
	edge.node = to;

	node->edgeCount = node->edgeCount + 1;

	Edge *edges = realloc(node->edges, node->edgeCount * sizeof(Edge));
	if (edges == NULL) {
		printError("%s", "Could not alloce enough memory for graph\n");
		return false;
	}
	edges[node->edgeCount - 1] = edge;

	node->edges = edges;
	return true;
}

bool nodeAddEdgeBothDirections(Node *node1, Node *node2, int distance) {
	return nodeAddEdge(node1, distance, node2) && nodeAddEdge(node2, distance, node1);
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
	}
	printf("%d ", node->value);
}

// void printGraphHeader(Graph *graph) {
// }
// 
// void printGraph(Graph *graph) {
// 	printGraphHeader(graph);
// }
//
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

void graphDtor(Graph *graph) {
	for (int i = 0; i < graph->size; i++) {
		free(graph->nodes[i].edges);
	}
	free(graph->nodes);
	graph->nodes = NULL;
	graph->size = 0;
}

Graph createGraphFromHeaderValues(List *headerValues) {
	Node *nodes = malloc(headerValues->size * sizeof(Node));
	for (int i = 0; i < headerValues->size; i++) {
		nodes[i] = nodeCtor(headerValues->values[i]);
	}
	return graphCtor(headerValues->size, nodes);
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