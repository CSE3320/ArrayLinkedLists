C=       	gcc
CFLAGS= 	-g -std=c99 -Wall -pedantic -ansi
CLFLAGS= 	-g -std=c99 -Wall -pedantic -ansi
LDFLAGS=
LIBRARIES=     

all:   LinkedListShifted LinkedListInPlace 

LinkedListInPlace: LinkedListInPlace.o 
	gcc -o LinkedListInPlace LinkedListInPlace.o -g

LinkedListInPlace.o: LinkedListInPlace.c
	gcc  -c  LinkedListInPlace.c -g

LinkedListShifted: LinkedListShifted.o 
	gcc -o LinkedListShifted LinkedListShifted.o -g

LinkedListShifted.o:LinkedListShifted.c
	gcc  -c  LinkedListShifted.c -g


clean:
	rm -f *.o *.a LinkedListInPlace LinkedListShifted

.PHONY: all clean
