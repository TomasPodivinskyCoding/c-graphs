#ifndef C_GRAPHS_PRINT_TABLE_H
#define C_GRAPHS_PRINT_TABLE_H

typedef struct {
    int colCount;
    int rowCount;
    char **headerValues;
    char ***rowValues;
} TableParams;

void printTable(TableParams *tableParams);
void freeTable(TableParams *tableParams);

#endif //C_GRAPHS_PRINT_TABLE_H
