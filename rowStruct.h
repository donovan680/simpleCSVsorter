#ifndef _ROWSTRUCT_H_
#define _ROWSTRUCT_H_

typedef struct _Row {
    char *values[40];
    struct _Row *next;
} Row;

#endif