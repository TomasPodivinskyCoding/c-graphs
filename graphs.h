#ifndef graphs
#define graphs

struct Node_;
struct Neighbour_;

typedef enum {
	VISITED,
	NOT_VISITED
} NodeStatus;

typedef struct Node_ {
	int value;
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
	Node **nodes;
	int size;
} Graph;

Node *nodeCtor(int value);
bool nodeAddEdge(Node *node, int distance, Node *to);
bool nodeAddEdgeBothDirections(Node *node1, Node *node2, int distance);
void nodeToInitialState(Node *node);
void nodePrint(Node *node);
void nodeBacktrackPrint(Node *node);
void graphDtor(Graph *graph);
Graph graphCtor(int size, Node **nodes);
Graph emptyGraph();

#endif
