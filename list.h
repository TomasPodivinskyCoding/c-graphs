#ifndef LIST_HEADER
#define LIST_HEADER

typedef struct {
	int *values;
	int capacity;
	int size;
} List;

List listCtor();
void listDtor(List *list);
bool listAppend(List *list, int num);

#endif
