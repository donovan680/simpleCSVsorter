all: simpleCSVsorter.c mergesort.o
	gcc -Wall -Werror -fsanitize=address simpleCSVsorter.c mergesort.o -o simpleCSVsorter

mergesort.o: mergesort.c
	gcc -c mergesort.c -o mergesort.o

clean:
	rm -rf simpleCSVsorter mergesort.o