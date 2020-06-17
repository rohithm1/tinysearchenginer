# Makefile for indexer
#
# Rohith Mandavilli, April 2019, CS50

L = ../libcs50
LP = ../common

PROG = indexer
OBJS = indexer.o 
PROG2 = indextest
OBJS2 = indextest.o
LLIBS = -lm $L/libcs50-given.a $(LP)/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(LP)
CC = gcc

all: $(PROG) $(PROG2)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

indexer.o: $(LP)/common.a #$L/libcs50-given.a
indextest.o: $(LP)/common.a

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
	rm -f $(PROG) $(PROG2) result test compare1 compare2
#delete all the files