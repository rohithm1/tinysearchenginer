# Makefile for indexer
#
# Rohith Mandavilli, April 2019, CS50

L = ../libcs50
LP = ../common

PROG = querier
OBJS = querier.o 
LLIBS = -lm $L/libcs50-given.a $(LP)/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(LP)
CC = gcc

all: $(PROG) $(PROG2)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

querier.o: $(LP)/common.a #$L/libcs50-given.a

.PHONY: all clean

#expects a file `testing.sh` to exist.
test: $(PROG)
	bash -v testing.sh 

# expects a file `test.names` to exist; it can contain any text.
valgrind: $(PROG) 
	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG) ../indexer/result ../indexer/test 
#delete all the files