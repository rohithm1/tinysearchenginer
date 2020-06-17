#!/bin/bash
# testing.sh - bash file for indexer.c
# Rohith Mandavilli, CS50 - David Kotz, May 2019
# Description: We need to test the thoroughness of our querier.c 

cd ../indexer
mkdir testDir

#Make sure crawler code has output to be used in the querier code
cd ../crawler/
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html ../indexer/testDir 1

# Normal code command to demonstrate functionality (non existing file to write to)
cd ../indexer/
./indexer testDir result

#bad file
cd ../querier/
valgrind ./quququqqu ../indexer/testDir ../indexer/result

#bad pathname
valgrind ./querier ../indexer/tesetDir ../indexer/result

#bad file to read from
valgrind ./querier ../indexer/testDir ../indexer/results

#Normal code to demonstrate functionality
echo dartmouth computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth or computer or science | valgrind ./querier ../indexer/testDir ../indexer/result

#Capital letters
echo DARTMOUTH OR COMPUTER OR SCIENCE | valgrind ./querier ../indexer/testDir ../indexer/result

#only white space
echo                 | valgrind ./querier ../indexer/testDir ../indexer/result

#complex query
echo dartmouth and computer science or fish and capital or home besides me | valgrind ./querier ../indexer/testDir ../indexer/result

#word doesnt exist in the index data struct
echo sciec | valgrind ./querier ../indexer/testDir ../indexer/result

#using numbers and bad characters
echo dartmouth 1234 computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth34 computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth computer1 science | valgrind ./querier ../indexer/testDir ../indexer/result

echo "dartmouth computer? science" | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth com!puter science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth computer sci//ence | valgrind ./querier ../indexer/testDir ../indexer/result

#syntax cases that should fail
echo and dartmouth computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo or dartmouth computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth and and computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth or or computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth computer science and | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth computer science or | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth and or computer science | valgrind ./querier ../indexer/testDir ../indexer/result

echo dartmouth or and computer science | valgrind ./querier ../indexer/testDir ../indexer/result


cd ../indexer
rm -rf testDir



