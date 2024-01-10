#include "char_utils.h"
#include <stdio.h>

int digitCount(int x) {
    int digitCount = 0;
    while (x > 0) {
        x /= 10;
        digitCount++;
    }
    return digitCount;
}
