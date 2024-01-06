#include "string_builder.h"
#include "char_utils.h"
#include <malloc.h>
#include <assert.h>
#include <math_utils.h>

String stringCtor() {
    String string;
    string.value = malloc(1 * sizeof(char));
    string.value[0] = '\0';
    string.capacity = 1;
    string.end = 0;
    return string;
}

void increaseCapacity(String *string, int factor) {
    string->capacity += factor;
    char *newString = realloc(string->value, string->capacity * sizeof(char));
    assert(newString != NULL && "Couldn't allocate enough memory\n");
    string->value = newString;
}

void stringAppendInt(String *string, int append) {
    int appendDigitCount = digitCount(append);
    increaseCapacity(string, appendDigitCount);
    int exponent = appendDigitCount - 1;
    do {
        char digitToAppend = (char) (append / (int) power(10, exponent));
        char digitCharToAppend = (char) ('0' + digitToAppend);
        string->value[string->end++] = digitCharToAppend;
        append /= 10;
    } while(append > 0);
    string->value[string->end] = '\0';
}

void stringAppendChar(String *string, char append) {
    increaseCapacity(string, 1);
    string->value[string->end++] = append;
    string->value[string->end] = '\0';
}

void stringReverse(String *string) {
    for (int i = 0; i < string->end / 2; i++) {
        char temp = string->value[i];
        string->value[i] = string->value[string->end - i - 1];
        string->value[string->end - i - 1] = temp;
    }
}