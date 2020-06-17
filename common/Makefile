# Makefile for pagedir in common
#
# Rohith Mandavilli, April 2019, CS50

L = ../libcs50

OBJS = index.o pagedir.o word.o
LIBS = common.a
LLIBS = -lm $L/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc

$(LIBS): $(OBJS) $(LLIBS)
	ar cr $(LIBS) $(OBJS)

pagedir.o: pagedir.h 
index.o: index.h
word.o: word.h

.PHONY: all clean

# expects a file `test.names` to exist; it can contain any text.
#test: $(PROG) 
#	./$(PROG)

# expects a file `test.names` to exist; it can contain any text.
#valgrind: $(PROG) 
#	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)