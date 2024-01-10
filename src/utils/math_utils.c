int max(int a, int b) {
    return a > b ? a : b;
}

int power(int value, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    for (int i = 0; i < exponent; i++) {
        value *= value;
    }
    return value;
}