#ifndef _SIMPLECSVSORTER_H_
#define _SIMPLECSVSORTER_H_
#include "mergesort.h"
#include "rowStruct.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// ALGORITHM FUNCTIONS

Row *createRow(char *);

void insertRow(Row **, char *);

// HELPER FUNCTIONS
char *removeNewline(char *);

void printRow(Row *, int);

void printTable(Row **, char **, int);

// FUNCTION DEFINITIONS

char *removeNewline(char *line) {
    char *new = strdup(line);
    int i;
    for (i = 0; i < strlen(new); i++) {
        if (new[i] == '\n' || new[i] == '\r') {
            new[i] = '\0';
        }
    }
    return new;
}

void printRow(Row *row, int columnNum) {
    int i;
    for (i = 0; i < columnNum; i++) {
        if (i == columnNum - 1) {
            fprintf(stdout,"%s\n", row->values[i]);
        } else {
            fprintf(stdout,"%s,", row->values[i]);
        }
    }
}

Row *createRow(char *string) {
    Row *row = (Row *)calloc(1, sizeof(Row));
    row->next = NULL;
    int columns = 0;

    char *token;
    char *bf = strdup(string);
    char *stringFix = (char *)calloc(256, sizeof(char));
    while ((token = strsep(&bf, ","))) {
        if (token[0] == '"') {
            while (token[strlen(token) - 1] != '"') {
                strcat(stringFix, token);
                strcat(stringFix, ",");
                token = strsep(&bf, ",");
            }
            strcat(stringFix, token);
            row->values[columns] = calloc(256, sizeof(char));
            strcpy(row->values[columns], stringFix);
            columns++;
            continue;
        }
        row->values[columns] = calloc(256, sizeof(char));
        token = removeNewline(token);
        strcpy(row->values[columns], token);
        columns++;
    }

    return row;
}

void insertRow(Row **topRow, char *string) {
    if (*topRow == NULL) {
        *topRow = createRow(string);
        return;
    }
    Row *ptr = *topRow;
    Row *prev = NULL;
    while (ptr != NULL) {
        prev = ptr;
        ptr = ptr->next;
    }
    prev->next = createRow(string);
}

void printTable(Row **topRow, char **header, int columnNum) {
    int i;
    for (i = 0; i < columnNum; i++) {
        if (i == columnNum - 1) {
            fprintf(stdout,"%s", header[i]);
        } else {
            fprintf(stdout,"%s,", header[i]);
        }
    }
    // PRINTS FOLLOWING ROWS
    Row *row = *topRow;
    while (row != NULL) {
        printRow(row, columnNum);
        row = row->next;
    }
}

#endif