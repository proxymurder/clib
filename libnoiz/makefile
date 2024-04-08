CFLAGS= -Wall -pedantic -g

all: libflex flextest

libflex: src/libflex.c
	gcc -c $(CFLAGS) src/libflex.c -o build/libflex.o

flextest: src/flextest.c
	gcc $(CFLAGS) -pthread src/flextest.c build/libflex.o -o build/flextest

