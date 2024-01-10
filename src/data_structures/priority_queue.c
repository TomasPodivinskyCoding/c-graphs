/**
 * Priority queue implementation for Dijkstra's algorithm
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "graphs.h"
#include "priority_queue.h"
#include "dprint.h"

#define elementDistance(index) queue->values[index]->distance

enum ChildType {
	LEFT = 1,
	RIGHT = 2
};

void nodePQExpand(NodePQ *queue) {
	if (queue->size > queue->capacity) {
		int newCapacity = queue->capacity + (queue->capacity / 2); 
		queue->values = realloc(queue->values, newCapacity * sizeof(Node*));
		assert(queue->values != NULL && "Couldn't allocate enough memory\n");
		queue->capacity = newCapacity;
	}
}

int getChildIndex(int index, enum ChildType type) {
	return 2 * index + type;
}

int getParentIndex(int index) {
	return (index - 1) / 2;
}

void nodePQSwap(NodePQ *queue, int first, int second) {
	Node *tmp = queue->values[first];
	queue->values[first] = queue->values[second];
	queue->values[second] = tmp;
}

NodePQ nodePQCtor(int capacity) {
	NodePQ queue;
	queue.size = 0;
	queue.capacity = capacity;
	queue.values = malloc(queue.capacity * sizeof(Node*));
	assert(queue.values != NULL && "Couldn't allocate enough memory\n");
	return queue;
}

void siftUp(NodePQ *queue, int elementIndex) {
	int parentIndex = getParentIndex(elementIndex);
	while (elementDistance(parentIndex) > elementDistance(elementIndex)) {
		nodePQSwap(queue, parentIndex, elementIndex);
		elementIndex = parentIndex;
		parentIndex = getParentIndex(elementIndex);
	}
}

bool shouldSwitchParentChild(NodePQ *queue, int parentIndex, int childIndex) {
	return (childIndex < queue->size && elementDistance(parentIndex) > elementDistance(childIndex));
}

void siftDown(NodePQ *queue, int elementIndex) {
	int leftChildIndex = getChildIndex(elementIndex, LEFT);
	int rightChildIndex = getChildIndex(elementIndex, RIGHT);
	
	while (shouldSwitchParentChild(queue, elementIndex, leftChildIndex) || shouldSwitchParentChild(queue, elementIndex, rightChildIndex)) {
		if (shouldSwitchParentChild(queue, elementIndex, leftChildIndex)) {
			nodePQSwap(queue, leftChildIndex, elementIndex);
			elementIndex = leftChildIndex;
		} else if (shouldSwitchParentChild(queue, elementIndex, rightChildIndex)) {
			nodePQSwap(queue, rightChildIndex, elementIndex);
			elementIndex = rightChildIndex;
		}
		leftChildIndex = getChildIndex(elementIndex, LEFT);
		rightChildIndex = getChildIndex(elementIndex, RIGHT);
	}
}

void nodePQInsert(NodePQ *queue, Node *element) {
	queue->size++;
	nodePQExpand(queue);
	queue->values[queue->size - 1] = element;
	siftUp(queue, queue->size - 1);
}

Node *nodePQPop(NodePQ *queue) {
	if (queue->size - 1 < 0) {
		return NULL;
	}

	Node *popped = queue->values[0];
	queue->values[0] = queue->values[queue->size - 1];
	queue->size--;

	siftDown(queue, 0);
	return popped;
}

int findFirst(NodePQ *queue, Node *element) {
	for (int i = 0; i < queue->size; i++) {
		if (queue->values[i] == element) {
			return i;
		}
	}
	return -1;
}

void nodePQUpdate(NodePQ *queue, Node *element, int newDistance) {
	int elementIndex = findFirst(queue, element);
	if (elementIndex == -1) {
		return;
	}

	int elementDistance = element->distance;
	element->distance = newDistance;
	if (newDistance > elementDistance) {
		siftUp(queue, elementIndex);
	} else {
		siftDown(queue, elementIndex);
	}
}

void nodePQDtor(NodePQ *queue) {
	free(queue->values);
}

