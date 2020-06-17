/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Rohith Mandavilli, April 2019, CS50
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include "bag.h"
#include "file.h"
#include "hashtable.h"
#include "pagedir.h"
#include <string.h>
#include <math.h>
#include "index.h"
#include "counters.h"
#include "word.h"
#include "memory.h"



/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct index{
	hashtable_t* table;
}index_t;  // opaque to users of the module


index_t *
index_new(const int numslots)
{
	if (numslots > 0) {
		//create the index for the struct
		index_t* index = count_malloc(sizeof(index_t));
		index->table = hashtable_new(numslots);
		return index;
	}
	return NULL;
}

counters_t*
index_find(index_t* index, char* word)
{
	if (index != NULL && word != NULL) {
		//return the value if it exists in the hashtable
		if (hashtable_find(index->table, word) != NULL) {
			return hashtable_find(index->table, word);
		}
	}
	return NULL;
}

bool
index_insert(index_t* index, char* word, counters_t* counter)
{
	if (index != NULL && word != NULL && counter != NULL) {
		//inserts the word and counterset into the hashtable
		if (hashtable_insert(index->table, word, counter)) {
			return true;
		}
	}
	return false;
}

int
index_save(FILE* file, index_t* index)
{
	if (file != NULL && index != NULL) {
		//saves the index into a file, called by hashtable_func
		hashtable_iterate(index->table, file, hashtable_func);
		fclose(file);
		return 0;
	}
	return 1;
}

void
hashtable_func(void *arg, const char *key, void *item)
{
	//print to the inputted file
	fprintf(arg, "%s ", key);
	counters_iterate(item, arg, counters_func);
	fprintf(arg, "\n");
}

void
counters_func(void *arg, const int key, const int count)
{
	fprintf(arg, "%d %d ", key, count);
}

void
index_delete(index_t* index)
{
	//if the index exists delete it
	if (index != NULL) {
		hashtable_delete(index->table, itemdelete);
		//make sure to free the index after everything in the index is freed
		free(index);
	}
}

void
itemdelete(void *item)
{
	//need to set the item to a counters_t* pointer
	counters_t *ctr = item;
	if (ctr != NULL) {
		counters_delete(ctr);
	}
}

int 
index_build(char* pathname, index_t* index)
{
	//defaults
	
	//first file in the directory
	int id = 1;

	//need to create a temporary filename so that the original pathname is not edited
	char* temp = malloc(strlen(pathname)+3+log10(id));
	sprintf(temp, "%s/%d", pathname, id);
	FILE* fp;

	//loop through each file in the directory
	while ((fp = fopen(temp, "r"))!= NULL) {

		//get defaults to create the page
		char* url = freadwordp(fp);
		char* temp1 = freadlinep(fp);
		int depth = atoi(temp1);	//converts the char* into an integer
		free(temp1);
		char* html = freadfilep(fp);
		webpage_t* page = webpage_new(url, depth, html);

		//char* to loop through the html
		char* word;
		int pos = 0;	//beginning position for each file

		//loop through the html
		while ((word = webpage_getNextWord(page, &pos)) != NULL) {

			//normalize the word
			char* normalWord;
			normalWord = normalizeWord(word);

			//if the normal word has length greater than 3, handle it
     		if (strlen(normalWord) >= 3) {

     			//if it already exists in the table, increment its count
	     		if (index_find(index, normalWord) != NULL) {
	     			counters_t* counter = index_find(index, normalWord);
	     			counters_add(counter, id);
	     		}

	     		//if its not existent, create a new one 
	     		else {
	     			counters_t* counter = counters_new();
	     			counters_add(counter, id);
	     			index_insert(index, normalWord, counter);
	     		}
     		}

     		//free mem
     		free(normalWord);
     		free(word);
 		}

 		//free mem/reset defaults
 		id++;
 		free(temp);
 		temp = malloc(strlen(pathname)+3+log10(id));
		sprintf(temp, "%s/%d",pathname,id);
 		webpage_delete(page);
 		fclose(fp);
	}
	free(temp);
	return 0;
}

int
index_load(FILE* file, index_t* index)
{
	char *word = malloc(200*sizeof(char));
	int id;
	int count;
	while (fscanf(file, "%s ", word) == 1) {
		counters_t* counter = counters_new();
		index_insert(index, word, counter);
		while (fscanf(file, "%d %d", &id, &count) == 2){
 			counters_set(index_find(index, word), id, count);
		}
		free(word);
		word = malloc(200*sizeof(char));
	}
	free(word);
	fclose(file);
	return 0;
}









