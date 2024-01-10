#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "ansii.h"

List listCtor() {
	List list;
	list.capacity = 0;
	list.size = 0;
	list.values = NULL;
	return list;
}

void listDtor(List *list) {
	free(list->values);
}

void listAppend(List *list, int num) {
	list->capacity++;
	list->size++;
	int *newListValues = realloc(list->values, list->size * sizeof(int));
	assert(newListValues != NULL && "Could not allocate enough memory.\n");
	newListValues[list->size - 1] = num;
	list->values = newListValues;
}

