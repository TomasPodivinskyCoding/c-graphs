#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

bool listAppend(List *list, int num) {
	list->capacity++;
	list->size++;
	int *newListValues = realloc(list->values, list->size * sizeof(int));
	if (newListValues == NULL) {
		printError("%s", "Could not allocate enough memory.\n");
		return false;
	}
	newListValues[list->size - 1] = num;
	list->values = newListValues;
    return true;
}

