#include "mergesort.h"

void mymergesort(Row **topRow, int columnNum) {

    Row *row = *topRow;
    Row *firstHalf;
    Row *secondHalf;

    if ((row == NULL) || (row->next) == NULL) {
        return;
    }

    split(row, &firstHalf, &secondHalf); // Split into two

    mymergesort(&firstHalf, columnNum);  // sort first half
    mymergesort(&secondHalf, columnNum); // sort second half

    *topRow = merge(firstHalf, secondHalf, columnNum); // merge two halves
}

Row *merge(Row *firstHalf, Row *secondHalf, int columnNum) {
    int cmp;
    Row *mergedRows = NULL;
    if (firstHalf == NULL) {
        return secondHalf;
    } else if (secondHalf == NULL) {
        return firstHalf;
    }
    if (isNumeric(firstHalf->values[columnNum]) == 1 &&
        isNumeric(secondHalf->values[columnNum]) == 1) {
        int numOne = atoi(firstHalf->values[columnNum]);
        int numTwo = atoi(secondHalf->values[columnNum]);
        cmp = numOne - numTwo;
    } else {
        cmp = strcmp(trim(firstHalf->values[columnNum]),
                     trim(secondHalf->values[columnNum]));
    }
    if (cmp == 0 || cmp < 0) {
        // strcmp
        mergedRows = firstHalf;
        mergedRows->next = merge(firstHalf->next, secondHalf, columnNum);
    } else {
        mergedRows = secondHalf;
        mergedRows->next = merge(firstHalf, secondHalf->next, columnNum);
    }
    return mergedRows;
}

void split(Row *row, Row **firstHalf, Row **secondHalf) {
    Row *ptr1 = row;
    Row *ptr2 = row->next;
    while (ptr2 != NULL) {
        ptr2 = ptr2->next;
        if (ptr2 != NULL) {
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
    }

    *firstHalf = row;
    *secondHalf = ptr1->next;

    ptr1->next = NULL;
}

char *trim(char *string) {
    char *tail;
    // Trim leading space
    if (*string == '"') {
        string++;
    }
    while (isspace(*string)) {
        string++;
    }

    if (*string == 0) {
        return string;
    }
    tail = string + strlen(string) - 1;
    while (tail > string && isspace(*tail)) {
        tail--;
    }
    tail[1] = '\0';

    return string;
}

int isNumeric(char *str) {
    int i = 0;
    if (str[i] == '\0') {
        return 0;
    }
    for (i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}
