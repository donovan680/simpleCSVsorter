#include "simpleCSVsorter.h"

int main(int argc, char **argv) {
    int errno;
    if (argc != 3) {
        fprintf(stderr, "Incorrent arguments! Please try again.\n");
        close(fileno(stdout));
        exit(errno);
    }
    if (strcmp(argv[1], "-c") != 0) {
        fprintf(stderr, "Incorrect arguments! Please try again.\n");
        close(fileno(stdout));
        exit(errno);
    }

    char string[10000];
    int numColumns = 0;
    char **header = (char **)calloc(40, sizeof(char *));
    Row *topRow = NULL;

    // CREATE HEADER
    if (fgets(string, 10000, stdin)) {
        char *token;
        char *bf = strdup(string);
        while ((token = strsep(&bf, ","))) {
            header[numColumns] = calloc(256, sizeof(char));
            strcpy(header[numColumns], token);
            numColumns++;
        }

        // OTHERWISE THROW ERROR BC FILE IS EMPTY
    } else {
        fprintf(stderr, "File is empty! Please try again.\n");
        close(fileno(stdout));
        exit(errno);
    }

    int headerNum;
    for (headerNum = 0; header[headerNum]!=NULL; headerNum++) {
        char *clean = removeNewline(header[headerNum]);
        if (strcmp(clean, argv[2]) == 0) {
            break;
        }
    }
    if (header[headerNum] == NULL) {
            fprintf(stderr,"Please enter a valid header!\n");
            close(fileno(stdout));
            exit(errno);

        }

    while (fgets(string, 10000, stdin)) {
        insertRow(&topRow, string);
    }

    mymergesort(&topRow, headerNum);

    printTable(&topRow, header, numColumns);

    return 0;
}
