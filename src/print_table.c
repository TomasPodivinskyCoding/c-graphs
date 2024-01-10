#include "../include/print_table.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "ansii.h"
#include "math_utils.h"

void printNTimes(char *value, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s", value);
    }
}

void printHeader(TableParams *tableParams, int *headerLengths) {
    printf(BOLDWHITE);
    for (int column = 0; column < tableParams->colCount; column++) {
        printf("| %-*s ", headerLengths[column], tableParams->headerValues[column]);
    }
    printf("|\n"RESET);
}

void printRows(TableParams *tableParams, int *headerLengths) {
    for (int row = 0; row < tableParams->rowCount; row++) {
        for (int col = 0; col < tableParams->colCount; col++) {
            printf("| %-*s ", headerLengths[col], tableParams->rowValues[row][col]);
        }
        printf("|\n");
    }
}

int maxColLength(TableParams *tableParams, int col) {
    int maxColLength = 0;
    for (int row = 0; row < tableParams->rowCount; row++) {
        int cellLength = (int) strlen(tableParams->rowValues[row][col]);
        if (cellLength > maxColLength) {
            maxColLength = cellLength;
        }
    }
    return maxColLength;
}

int *headerValuesLengths(TableParams *tableParams) {
    int *headerLengths = malloc(tableParams->colCount * sizeof(int));
    assert(headerLengths != NULL && "Couldn't allocate enough memory\n");
    for (int col = 0; col < tableParams->colCount; col++) {
        headerLengths[col] = max(maxColLength(tableParams, col), (int) strlen(tableParams->headerValues[col]));
    }
    return headerLengths;
}

int headerRowPadding(TableParams *tableParams) {
    int celPaddingLength = strlen("|  |");
    int totalPadding = (tableParams->colCount * celPaddingLength);
    int sharedPadding = max(0, tableParams->colCount - 1);
    return totalPadding - sharedPadding;
}

int totalRowLength(TableParams *tableParams, const int *headerLengths) {
    int totalLength = 0;
    for (int i = 0; i < tableParams->colCount; i++) {
        totalLength += headerLengths[i];
    }
    return totalLength + headerRowPadding(tableParams);
}

void printTable(TableParams *tableParams) {
    int *headerLengths = headerValuesLengths(tableParams);
    int totalLength = totalRowLength(tableParams, headerLengths);
    char *divider = "-";
    printNTimes(divider, totalLength);
    printf("\n");

    printHeader(tableParams, headerLengths);
    printRows(tableParams, headerLengths);

    printNTimes(divider, totalLength);
    printf("\n");

    free(headerLengths);
}

TableParams tableParamsCtor(char **headerValues, int colCount, int rowCount, char ***rowValues) {
    TableParams tableParams;
    tableParams.headerValues = headerValues;
    tableParams.colCount = colCount;
    tableParams.rowCount = rowCount;
    tableParams.rowValues = rowValues;
    return tableParams;
}

void freeTableParams(TableParams *tableParams) {
    for (int row = 0; row < tableParams->rowCount; row++) {
        for (int col = 0; col < tableParams->colCount; col++) {
            free(tableParams->rowValues[row][col]);
        }
        free(tableParams->rowValues[row]);
    }
    free(tableParams->rowValues);
}