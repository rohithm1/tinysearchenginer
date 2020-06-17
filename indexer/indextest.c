/*
 * Indextest.c - see Implentation.md and Design.md for more details
 *
 * usage: ./indextest [file1] [file2]
 * 	where file 1 is the output of indexer and file2 is the file we want to save
 * 	the output of indextest to.
 * stdin: 
 * stdout: 
 *
 * Non-zero exit status if invalid input/error and 0 if success.
 * Rohith Mandavilli, May 2019, CS50
 *
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


int
main(int argc, char *argv[])
{
	FILE* filename1;
	FILE* filename2;
	index_t* index;
	index = index_new(200);
	int exit = 0;

	if (argc != 3) {
		printf("Incorrect arguments: Usage:./indextest [file1] [file2]\n");
		exit = 1;
	}
	if (exit == 0) {
		if ((filename1 = fopen(argv[1], "r")) != NULL) {
			index_load(filename1, index);
			if ((filename2 = fopen(argv[2], "w")) != NULL) {
				exit = index_save(filename2, index);
			}
			else {
				printf("%s is not an openable/readable file.\n", argv[2]);
			}
		}
		else {
			printf("%s is not an openable/readable file.\n", argv[1]); 
			exit = 1;
		}
	}
	
	index_delete(index);

	return exit;
}



