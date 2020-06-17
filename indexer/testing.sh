#!/bin/bash
# testing.sh - bash file for indexer.c
# Rohith Mandavilli, CS50 - David Kotz, May 2019
# Description: We need to test the thoroughness of our indexer.c and our indextest.c Code


#Running crawler code to be able to run the indexer code
mkdir testDir
touch foo

#run the crawler function to create the files
cd ../crawler/
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html ../indexer/testDir 1

#indexer.c test cases + boundary cases

# Normal code command to demonstrate functionality (non existing file to write to)
cd ../indexer/
valgrind ./indexer testDir result

#bad number of arguments

valgrind ./indexer testDir result thisisarandom

#bad indexer format

valgrind ./indesssssssser testDir result

#bad pathdir

valgrind ./indexer loldir result


#file is unwriteable

chmod -w foo
valgrind ./indexer testDir foo

#existing file to write to

valgrind ./indexer testDir result

#indextest.c test cases + boundary cases

# Normal code command to demonstrate functionality (non existing file to write to)

valgrind ./indextest result test

# bad number of arguments

valgrind ./indextest result test lolwaht

#bad indextest format

valgrind ./indextestihfoehfodquWHF result test

#non existing filename to read from 

valgrind ./indextest resusfqlt test

#unwritable file to write to 

valgrind ./indextest result foo

#get a 0 exit status

valgrind ./indextest result test

rm -f foo

#sort to compare
touch compare1
touch compare2

#sort the files
gawk -f indexsort.awk result > compare1
gawk -f indexsort.awk test > compare2

#compare the two
#Here are the differences between the files
diff compare1 compare2

rm -rf testDir





