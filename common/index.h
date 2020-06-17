/* 
 * index.h - header file for CS50 index module
 *
 *
 * Rohith Mandavilli April 2019
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "counters.h"
#include "hashtable.h"


typedef struct index index_t;  // opaque to users of the module

/**************** index_new ****************/
/* 
 * Creates the new index.
 * returns the created index, if successful, returns NULL if fails
 * 
 */
index_t * index_new(const int numslots);

/**************** index_find ****************/
/* 
 * Finds the input word in the input index.
 * returns the counters_t* associated with that char* word if exists
 * returns NULL if it is nonexistent
 * 
 */
counters_t* index_find(index_t* index, char* word);

/**************** index_insert ****************/
/* 
 * inserts the input char* word and counters_t* counter into the inputer index_t*.
 * will input duplicates, and will hash to the same value if the words are the same
 * 	if the words are the same, the counter will have a different id number associated 
 *	with different files
 * returns true if the insert was successful
 * returns false if it the insert failed - will happen if null inputs
 * 
 */
bool index_insert(index_t* index, char* word, counters_t* counter);

/**************** index_save ****************/
/* 
 * saves the index into a file with a formatted print
 * will print in the following way: "word %d %d"
 * 	where the first %d is the file id, and the second in the amount of times it appears
 * returns NULL if it is nonexistent
 * 
 */
int index_save(FILE* file, index_t* index);

/**************** hashtable_func ****************/
/* 
 * prints the key input and calls counters_iterate to move through each counter set
 * 
 */
void hashtable_func(void *arg, const char *key, void *item);

/**************** counters_func ****************/
/* 
 * prints the unique id of the file for each word followed by the count that the word
 *	appears in the file
 * 
 */
void counters_func(void *arg, const int key, const int count);

/**************** index_delete ****************/
/*
 * deletes the input index if if its not already NULL
 */
void index_delete(index_t* index);

/**************** itemdelete ****************/
/*
 * deletes the input item if if its not already NULL
 */
void itemdelete(void *item);

/**************** index_build ****************/
/*
 * takes all the text outputted from the crawler directory and takes it as input
 * uses that crawler output to input data into the inverted data structure 
 * will hold the amount of times each word appears in each file
 */
int index_build(char* pathname, index_t* index);

/**************** index_load ****************/
/*
 * grabs data from the file inputted and parses it into the index
 */
int index_load(FILE* file, index_t* index);
#endif






