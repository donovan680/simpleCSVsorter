#ifndef _MERGESORT_H_
#define _MERGESORT_H_
#include "rowStruct.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SORTING FUNCTIONS

void split(Row *row, Row **firstHalf, Row **secondHalf);

Row *merge(Row *firstHalf, Row *secondHalf, int columnNum);

void mymergesort(Row **, int);

char *trim(char *);

int isNumeric(char *);

#endif