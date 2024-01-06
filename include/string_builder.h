#ifndef C_GRAPHS_STRING_BUILDER_H
#define C_GRAPHS_STRING_BUILDER_H

typedef struct {
    char *value;
    int capacity;
    int end;
} String;

String stringCtor();
void increaseCapacity(String *string, int factor);
void stringAppendInt(String *string, int append);
void stringAppendChar(String *string, char append);
void stringReverse(String *string);

#endif //C_GRAPHS_STRING_BUILDER_H
