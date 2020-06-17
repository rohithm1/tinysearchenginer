#!/bin/bash
# testing.sh - bash file for crawler.c
# Rohith Mandavilli, CS50 - David Kotz, April 2019
# Description: We need to test the thoroughness of our crawler.c Code


# Normal code commands to demonstrate functionality

#crawl at 0 depth
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html test 0

make cleanTests

#1 depth
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html test 1

make cleanTests

./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html test 1

make cleanTests

#2 depth
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html test 2

make cleanTests

# Check argument boundary cases

#bad pathname
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html what 1

make cleanTests

#extra random arguments
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html test 1 hello hello

make cleanTests

#bad depth
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html test -4

make cleanTests

./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html test 14

make cleanTests

#external URL
./crawler http://www.youtube.com/ test 1

make cleanTests

#nonexistent server
./crawler http://lolthisdoesntexist/ test 1

make cleanTests

#nonexistent page
./crawler http://www.youtube.com/lolthisdoesntexist test 1

make cleanTests

#internal URL with nonexistent page
valgrind ./crawler http://old-www.cs.dartmouth.edu/~campbell/inesesdex.html test 1


