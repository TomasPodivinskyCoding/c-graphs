#ifndef GRAPHS_HEADER 
#define GRAPHS_HEADER

struct Node_;
struct Neighbour_;

typedef enum {
	VISITED,
	NOT_VISITED,
	CLOSED
} NodeStatus;

typedef struct Node_ {
	int value;
	int distance;
	int edgeCount;
	NodeStatus status;
	struct Node_ *parent;
	struct Edge_ *edges;
} Node;

typedef struct Edge_ {
	int distance;
	Node *node;
} Edge;

typedef struct {
	Node *nodes;
	int size;
} Graph;

Node nodeCtor(int value);
void nodeAddEdge(Node *node, int distance, Node *to);
void nodeToInitialState(Node *node);
void nodePrint(Node *node);
void nodeBacktrackPrint(Node *node);
void graphDtor(Graph *graph);
Graph graphCtor(int size, Node *nodes);
Graph emptyGraph();
void appendNode(Graph *graph, Node *node);
void printGraphValues(Graph *graph);
void printGraphValidValuesError(Graph *graph);
Node *getGraphNode(Graph *graph, int value);

#endif
