#ifndef PRIORITY_QUEUE_HEADER
#define PRIORITY_QUEUE_HEADER

typedef struct {
	Node **values;
	int size; 
	int capacity; 
} NodePQ;

NodePQ nodePQCtor(int capacity);
void nodePQInsert(NodePQ *queue, Node *element);
Node *nodePQPop(NodePQ *queue);
void nodePQUpdate(NodePQ *queue, Node *element, int newDistance);
void nodePQDtor(NodePQ *queue);

#endif
