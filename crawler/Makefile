# Makefile for crawler
#
# Rohith Mandavilli, April 2019, CS50

L = ../libcs50
LP = ../common

PROG = crawler
OBJS = crawler.o 
LIBS = -lm
LLIBS = $L/libcs50-given.a $(LP)/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(LP)
CC = gcc

all: $(PROG)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

crawler.o: #$(LP)/common.a

.PHONY: all clean

#expects a file `testing.sh` to exist.
test: $(PROG)
	bash -v testing.sh 

# expects a file `test.names` to exist; it can contain any text.
#valgrind: $(PROG) 
#	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

cleanTests:
	rm -rf testDir/*