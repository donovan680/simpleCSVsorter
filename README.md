# scannerCSVsorter: C Program
This C program reads CSV filse from a directory and exports a sorted CSV based on a given column

## Required Files
`simpleCSVsorter.h`: This file contains function prototype declarations used in `simpleCSVsorter.c`.

`simpleCSVsorter.c`: This file contains the main() function.

`mergesort.h`: This file contains function prototype declarations used in `mergesort.c`

`mergesort.c`: This file contains library functions required for the sorting algorithm.

`rowStruct.h` This file contains the `Row` and `PIDLIST`struct

`Makefile`: This file contains build instructions for the compiler.

## Libraries Used

`<ctype.h>`: Used for isdigit() and isalpha()

`<stdio.h>`: Used for printf() and fprintf()

`<stdlib.h>`: Used for malloc(), calloc(), atoi()

`<string.h>`: Used for strcmp() and strcpy()

`<unistd.h>`: Used for close() and fclose()

`<errno.h>`: Used for errno variable

`<getopt.h>`: Used for getopt()

`<dirent.h>`: Used for opendir() and readdir()

`<sys/mman.h>`: Used for mmap()

`<sys/wait.h>`: Used for wait()


## Algorithm

In order to run scannerCSVsorter from the command line, the user must input the following:

 ./sorter -c header_to_sort_by

### Required Parameters

`input.file`: This is the CSV file to which the program will sort. This file must be a valid CSV with columns separated by commas and rows separated by newlines.

`-c`: This argument tells the program that it should sort by columns.

`header_to_sort_by`: This argument tells the program which column will be sorted. The argument must be a valid header in the CSV or the program will print an error and the program will halt.

`output.file`: This is where the sorted CSV file will be created.

### Optional Parameters

 `-d <inputDirectory>`: This tells the program where to search for CSV files. The program will fork into a child process each time it reaches a directory or a file. If this parameter is left out, the program will begin to search at its current directory.
 
 `-o <outputDirectory>`: This tells the program where to output sorted CSV files. If this is left out, the program will output CSV files in the same directory that they were found.
 
 The parameters may be entered in any order as long as they are followed by valid arguments.
 
 ie:
 ```
 ./scanner -c movie_title -o sortedFiles
 
 ```
 is equivalent to
 
 ```
  ./scanner -o sortedFiles -c movie_title 

 ```

### Execution

The program begins with a traversal through a directory. The program uses opendir() to create a directory stream of all the values inside of the current directory. The stream then increments through each value inside of the directory and creates a fork process for each. If the process is forked on a sub-directory, the function is ran recursively. If the process is forked on a file, it checks to see if the file is a valid .CSV file. If so, it is sorted and outputted in the proper location. The sorting is done as follows:


The program will begin by loading the header names into `char *header[40]`. Since this is a string array of static size 40, the number of headers in the CSV must be less than or equal to 40. This global variable will be used as a reference when determining which column will be sorted.

After loading the header array, the program will continue to read from the CSV file line by line. Once a line is read, it is stored as a string defined as `char string[10000]`. Therefore, the number of characters in each line must be less than or equal to 10000. Since each line represents a row in the CSV file, a new `Row` struct is created each time that a new line is read (see more about `struct Row` below). The string is then separated by commas, excluding those between double quotes `","`, and each substring will represent different column values of that row. These substrings are loaded into the `char* values[40]` array defined by the `Row` struct. The `Row` is then either linked to the previous `Row` or set to `Row * topRow` is `topRow==NULL`. 

Once all lines of the CSV file have been read, the program has created a linked list of `Row` structs with each `Row` holding the string values of each column in that given row. At this point, the program will sort the linked list using mergesort on the `header_to_sort_by` values of each `Row`.

With the linked list of `Row` structs now sorted by the given `header_to_sort_by` column, the program then prints the values of `header` followed by the column values listed in each `Row`. 

This output stream is directed to a file defined by `<file-name>-sorted-<fieldname>.CSV`.

In STDOUT, following information is displayed:

```
Initial PID: 30719
PIDS of all child processes: 30721,30722,30723,30724,30725,30726,30727
Total number of processes: 7

```
The total number of processes is the number of times that the program created a forked process. 

All statuses on each fork process are sent to a `console.log` file. This file gives a description of how the forked process handeled each file.


## Structures

### Row

This structure is the fundamental building block of the algorithm.
```
typedef struct _Row{
  char *values[40];
  struct _Row *next;
} Row;
```
Each row is a linked list node which contains two fields:

`char *values[40]`: This is an array of strings which contains the values of the row. Each array index corresponds to the columns defined when header values are loaded. For example, if `header[5]` holds `"movie_title"`, then 
`Row->values[5]` might hold `"The Lord of the Rings"`

`struct _Row *next;`: This is a pointer to the next `Row`;

### PIDLIST

This structure was a global variable in shared memory for each of the processes. This is how the program keeps track of the number of child processes. 

```
typedef struct _PID {
    int count;
    pid_t list[300];
} PIDLIST;

```

## Functions

### Functions defined in `simpleCSVsorter.h`

#### `void traverseDirectory(char *directoryName, char *header, PIDLIST *shmem, char outputDir[1000]);`

This function is what traverses through each file and directory in the input stream. Each time it reaches another file or directory, a forked process is created to handle it. In each child process, this function is run recursively on sub-directories. 

#### `Row * createRow(char* line);`

This function takes a `char*` as a parameter and returns a pointer to a `Row` struct. The value of the `char*` parameter is an entire CSV file line. This parameter is passed down from the `insertRow` function defined below. In this function, a new `Row` struct is created. The `char*` is then split by `,` and each substring is loaded into the correct 
`Row* newRow->values[columnNum]`. `columnNum = 0` initially and is incremented for each substring loaded. Therefore each `Row*->values[columnNum]` stays consistent with the corresponding `header[columnNum]` column name. A pointer to the newly created `Row` is then returned.

#### `void insertRow(Row** topRow,char* line);`

This function links the `Row` created in the previous function to the list of `Row` structs. The `Row**` parameter is a reference to the `Row *topRow`. If `*topRow == NULL`, then `*topRow = createRow(char* line)` where `char* line` is an entire line read from the CSV file. If `*topRow!=NULL`, then the function iterates `Row *lastRow` through the linked list of `Row` structs until it has found the last `Row` in the list. Then `lastRow->next = createRow(char* line)`. 

#### `char * removeNewline(char* line);`

This function takes a `char*` as a parameter and removes the `'\n'` character from the end of the string. This function was used as a `'\n'` is automatically appended to the final column value of each `Row` during the algoritm. This was done because the final row in the CSV file does not contain a `'\n'` character and each row must be kept uniform. 

#### `void printRow(Row* row, int columnNum);`

This function takes a `Row *row` parameter and an `int columnNum` parameter where `row` is a reference to a specific `Row` in which the column values are to be printed. `columnNum` is the number of columns loaded into `header`. This function iterates through `row->[column]` where `column` increments from `0` to `columNum`. Each `row->[column]` is then printed.  

#### `void printTable(Row** topRow, char** header, int columnNum);`

This function prints the `header` column values followed by the sorted `Row` column values. `columnNum` is the number of values loaded into `header`. The function takes `char ** header` as a parameter and prints each `header[column]` value where `column` is incremented from `0` to `columnNum`. The function also takes `Row ** topRow` as a paremeter where `topRow` is a pointer to the first `Row *` in the linked list. After printing the header, a pointer `Row *ptr` is iterated through each `Row` of the linked list and `printRow(ptr,columnNum)` is ran for each iteration. This function is what prints the final sorted CSV file to stdout. 

### Merge Sort Functions defined in `mergesort.h`

#### `void mymergesort(Row **topRow, int columnNum);`

This function is the recursive mergesort used in this program. This function uses the `split` and `merge` functions (see below).

This function takes two parameters, `Row **topRow` and `int columnNum`. `topRow` is a reference to the first `Row*` of the list and `columnNum` is the column in which the program will be sorting by. The base cases of this recursive function are 
`*topRow == NULL` and `(*topRow)->next == NULL`. In either of these cases, no sorting must be done since there is either no or only one `Row`. In these cases, the function will return. If these cases are not met, `split(*topRow, &firstHalf,&secondHalf)` will be run in order to split the current list into two halves. Once the list has been split, `mymergesort(&firstHalf,columnNum)` and `mymergesort(%secondHalf,columnNum)` will be run recursively. After these lists have been sorted, `*topRow=merge(firstHalf,secondHalf,columnNum)` will be ran to set `*topRow`to be the sorted merged lists. 

#### `void split(Row *row, Row **firstHalf, Row **secondHalf);`

This function splits the given linked list headed by `Row *row` into two linked lists referenced by `Row **firstHalf` and `Row ** secondHalf`. This algorithm works by iterated two pointers through the linked list headed by `row`. In this algorithm, one pointer is iterated twice as fast as the second, and therefore when the second pointer is at null, the first pointer will be at the halfway point of the list. `*firstHalf` is the set to `row` and `*secondHalf` is set to the first (slow) pointer.

#### `Row *merge(Row *firstHalf, Row *secondHalf, int columnNum);`

This function is the fundamental sorting algorithm of the merge sort. `firstHalf` and `secondHalf` are the two linked lists in which this function is to be merging.The `int columnNum` value is the column in which the program is to be sorting on. This function is recursively defined with the base cases being `firstHalf==NULL` or `secondHalf==NULL`. If `firstHalf==NULL`, then `secondHalf` is returned and vice versa. If not, then the values at `firstHalf->values[columnNum]` and 
`secondHalf->values[columnNum]` are compared. In the case that these values are numeric, the integer values are subtracted to determine the larger value. In the case that these values are alphabetic, `strcmp` is used. The rest of the algorithm is a standard merge sort based on the value of the comparison. 

#### `char *trim(char* string);`

This function takes a `char*` as a parameter and returns the string with leading and trailing whitespace removed. This function is useful when using `strcmp` during the mergesort. 
For example:
```
char* string1 = "   whitespace   ";
char* string2 = trim(string1);
// string2 = "whitespace"
```

#### `int isNumeric(char * string);`

This function takes a `char*` as a parameter and returns 0: if the string is not a number
                                                 returns 1: if the string is a number
                                                 
This is useful when comparing column values during the mergesort. If `strcmp` is used for numerical values, then `111` would be regarded as less that `22`. Therefore if a value is found to be numerical, the integer values are compared instead. 

## Additional Information

### Assumptions

It is assumed that the CSV given has proper format. This means that each column in each row is separated by a single comma. The only time a comma can be included in a field is if it is surrounded by double quotes. In addition, it is assumed that the CSV will have less than 40 header entries. If there are more headers than this, the proram will break. Also, it is assumed that each line has less thatn 10,000 characters as this is the maximum string defined. 

### Difficulties

The largest difficulty was organizing the structure of the application. Linking the header files was difficult as there was circular dependency at first. This is why I chose to create `helper.h` so that I would not get duplicate symbol errors. In addition, there were many edge cases that I had to test for in order to make sure that the program was working.

In our program, we take used the same basic structure as the last submission. We have a scannerCSVsorter.h which includes all the method definitions outside of main, a scannerCSVsorter.c which defines the main method, a mergesort.c which defines and implements our mergesort program, and a mergesort.h which defines the variables and methods we used. We broke apart each line of the CSV into a struct "Row" (defined in rowStruct.h) which holds the line data as well as a pointer to the next line. The program takes in the input through stdin and first checks to see the arguments given. Using the getopt() function (called from the handleArguments() method), we are able to check for 'c', 'o', and 'd' arguments. After determining the course of action from the arguments, our program breaks apart the file into the header and a linked list of struct Rows. We mergesort this linked list to created the sorted CSV which is outputted with the -sorted extension to the directory it was found in. This process is repeated for each directory (starting with the current directory which serves as a root) and a new child process with a unique pid is created for each csv found and sorted. The sorted files as well as the pids of the processes used to create them are the expected output of the program.

We ran into some problems during our coding. We were not able to correctly increment some pointers, so we worked around this by passing in integers in their natural form. This actually helped improve the readability of the code as well. The biggest problem we encountered was having some processes start (and hence some unexpected PIDs outputted) when we did not want them to. We found that this was a result of the way the fork() function works. For every fork() call, a new stack of memory is created which can quickly lead to errors like the one we encountered. We fixed this by creating a custom create_shared_memory() function which utilizes the mmap() function in order to create a shared memory space for all forked processes to use. Once we implemented this, our program began outputting the expected child process details. We also had trouble getting the file to read in properly first, and then we remembered we had to "rewind" the stream in order to read in from the front of the file. There was an error we encountered where the files were not writing at all and this was due to us never closing the file stream at the end of our print to file methods. We ran into a problem in which our pidcount was altered by a hidden file, we fixed this by checking to see if a file starts with ".". In order to make sure our project runs even without a directory specified, we made sure to use the getcwd() function so that we can start the processes at the current file path.

### Testing Procedure

In order to test, I utilzied the `movie_metadata.csv` file. Since this file was so large, I assumed that it contained many of the cases that the program would encounter in other CSV files. I sorted each column in the CSV and checked to see if it had sorted correctly. I also used printf() statements throughout my code to make sure that certain functions were being hit. 

We tested multiple test cases: 

  We checked to see if all the flag combinations work and create files in the correct specified (or if unspecified in the cwd)             directory.
  We checked to see if the program correctly recognizes files as csv type files
  We checked to see if the files that are recognized as csv files are correctly sorted
  We checked with 5 levels of directories and subdirectories and checked to make sure each directory and file had a specific pid and was   forked.


