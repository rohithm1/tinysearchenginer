/*
 * Indexer.c - see Implentation.md and Design.md for more details
 *
 * usage: pagedir and filename to write to
 * stdin: 
 * stdout: 
 * 
 * Non-zero exit status if invalid input/error and 0 if success.
 * Rohith Mandavilli, April 2019, CS50
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


int indexer(char* pathname, char* filename);


int
main(int argc, char *argv[])
{
	//process arguments

	//make sure the user inputs the correct number of arguments
	if (argc != 3) {
		printf("Incorrect arg amount\n");
		return 1;
	}

	//set default exit status
	int exit = 0;

	//grab the pathname of the directory
	char *pathname = (char*)malloc((1 + strlen(argv[1]))*sizeof(char));
	strcpy(pathname, argv[1]);

	//create the testing string to feed into checkName() in pagedir.c
	char *pathtest = (char*)malloc((10 + strlen(argv[1]))*sizeof(char));
	strcpy(pathtest, argv[1]);
	strcat(pathtest, "/.indexer");

	//create the file name to write to
	char *filename = (char*)malloc((1 + strlen(argv[2]))*sizeof(char));
	strcpy(filename, argv[2]);

	//need to check if the directory currently exists
	if (!checkName(pathtest)) {
		printf("Pathname doesn't exist\n");
		exit = 1;
	}

	//if there have not been any errors, start the indexer
	if (exit == 0) {
		exit = indexer(pathname, filename);
	}

	//free data
	free(pathname);
	free(pathtest);
	free(filename);

	return exit;
}

/*
 * Handles the indexing to make the program cleaner
 */
int
indexer(char* pathname, char* filename)
{
	//defaults set
	int exit = 0;
	const int numslots = 200;

	//open the file to start writing to
	FILE* file;

	if ((file = fopen(filename, "w")) == NULL) {
		exit = 1;
		printf("The file is not writable\n");
	}

	//create the inverted index
	index_t* index;
	index = index_new(numslots);

	//input value into the data structure
	exit = index_build(pathname, index);
	if (exit == 0) {	
		//save it to a file
		exit = index_save(file, index);
	}
	else {
		fclose(file);
	}

	//delete data struc and close the file
	index_delete(index);
	//fclose(file);
	return exit;
}









