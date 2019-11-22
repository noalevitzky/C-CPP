CC= gcc
CFLAGS= -Wextra -Wvla -Wall -g -lm -std=c99

all: NumericalAnalyzer

# create main program
NumericalAnalyzer: NumericalAnalyzer.o libinfi.a
	$(CC) $(CFLAGS) NumericalAnalyzer.o -L . -linfi -o NumericalAnalyzer

NumericalAnalyzer.o: NumericalAnalyzer.c
	$(CC) $(CFLAGS) -c NumericalAnalyzer.c infi.h

infi.o: infi.c infi.h
	$(CC) $(CFLAGS) -c infi.c -o infi.o

# create infi library
libinfi.a: infi.o infi.h
	ar rcs libinfi.a infi.o

# clean
.PHONY: clean
clean:
	rm -f NumericalAnalyzer *.o *.a

