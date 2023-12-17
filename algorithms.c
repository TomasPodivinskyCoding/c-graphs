#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphs.h"

Node *bfs(Node *startNode, Node *endNode, int nodeCount) {
	int currentElementIndex = 0;
	int lastElementIndex = 1;
	Node *queue[nodeCount];
	startNode->status = VISITED;
	queue[0] = startNode;
	while (currentElementIndex < lastElementIndex && currentElementIndex < nodeCount) {
		Node *currentNode = queue[currentElementIndex];
		for (int i = 0; i < currentNode->edgeCount; i++) {
			Node *currentEdge = currentNode->edges[i].node;
			if (currentEdge->status == VISITED) {
				continue;
			}
			currentEdge->parent = currentNode;
			if (currentEdge == endNode) {
				return currentEdge;
			}
			currentEdge->status = VISITED;
			if (lastElementIndex >= nodeCount) {
				fprintf(stderr, "Node count too low\n");
				return NULL;
			}
			queue[lastElementIndex] = currentEdge;
			lastElementIndex++;
		}
		currentElementIndex++;
	}
	return NULL;
}

Node *dfs(Node *startNode, Node *endNode, int nodeCount) {
	int top = 0;
	Node *stack[nodeCount];
	startNode->status = VISITED;
	stack[0] = startNode;
	while (top < nodeCount) {
		Node *currentNode = stack[top--];
		for (int i = 0; i < currentNode->edgeCount; i++) {
			Node *currentEdge = currentNode->edges[i].node;
			if (currentEdge->status == VISITED) {
				continue;
			}
			currentEdge->parent = currentNode;
			if (currentEdge == endNode) {
				return currentEdge;
			}
			currentEdge->status = VISITED;
			if (++top >= nodeCount) {
				fprintf(stderr, "Node count too low\n");
				return NULL;
			}
			stack[top] = currentEdge;
		}
	}
	return NULL;
}
