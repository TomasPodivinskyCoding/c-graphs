#ifndef C_GRAPHS_PRINT_TABLE_H
#define C_GRAPHS_PRINT_TABLE_H

typedef struct {
    int colCount;
    int rowCount;
    char **headerValues;
    char ***rowValues;
} TableParams;

void printTable(TableParams *tableParams);
TableParams  tableParamsCtor(char **headerValues, int colCount, int rowCount, char ***rowValues);
void freeTableParams(TableParams *tableParams);

#endif //C_GRAPHS_PRINT_TABLE_H
