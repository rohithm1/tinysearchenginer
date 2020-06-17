/*
 * querier.c - see Implentation.md and Design.md for more details
 *
 * usage: ./querier pageDirectory indexFilename
 * 	where pageDirectory is the pathname of a directory produced by the Crawler, and
 *	where indexFilename is the pathname of a file produced by the Indexer
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
#include <ctype.h>
#include <unistd.h>


int querier(char* pathname, index_t* index);
bool is_empty(char* word);
bool valid_query(char* query);
counters_t* calculate_score(char* query, index_t* index);
void score(void *arg, const int key, const int count);
void rake(void *arg, const int key, const int count);
void initialize(void *arg, const int key, const int count);
void querierPrint(counters_t* scores, char* pathname);
void sizeCalc(void *arg, const int key, const int count);
void check(void *arg, const int key, const int count);
void highCalc(void *arg, const int key, const int count);
static void prompt(void);
int fileno(FILE *stream);

//new data structure that contains the id and each count
typedef struct docPrint {
  int id;
  int count;
} docPrint_t;

int
main(const int argc, char *argv[])
{
	//process arguments

	//make sure the user inputs the correct number of arguments
	if (argc != 3) {
		printf("Incorrect arg amount\n");
		return 1;
	}

	//set default exit status
	int exit = 0;
	FILE* fp;

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

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("The file is not readable/existing\n");
		exit = 2;
	}

	if (exit == 0) {
		index_t* index;
		index = index_new(200);
		index_load(fp, index);

		exit = querier(pathname, index);
		//printf("reached\n");

		index_delete(index);
	}
	else if (exit == 1) {
		fclose(fp);
	}
	
	//free to memory	
	free(pathname);
	free(pathtest);
	free(filename);
	
	return exit;
}

/*
 * Handles the querying to make the program cleaner
 */
int
querier(char* pathname, index_t* index)
{
	//set exit status
	int exit = 0;

	char* query;
	prompt(); //query print for the user

	//read from stdin
	while ((query =freadlinep(stdin)) != NULL) {
		
		//if the query is not empty
		if (strlen(query) > 0 && !is_empty(query)) {

			//make the query lower case
			char* normalQuery;
			normalQuery = normalizeWord(query);

			//make sure the query is in accordance with the syntax
			if (valid_query(normalQuery)) {
				printf("Query in use: %s\n", normalQuery);

				//make all the scores for each id
				counters_t* scores;
				scores = calculate_score(normalQuery, index);

				printf("\n");

				//print out the results of the query to the user
				querierPrint(scores, pathname);
				counters_delete(scores);
			}
			free(normalQuery);
		}
		prompt();
		free(query);
	}

	return exit;

}

/*
 * used to check if the input query is empty or not
 * will iterate through each character and make sure it is not a space
 */
bool
is_empty(char* query) 
{
	bool empty = true;
	int i = 0;
	for (; i < strlen(query); i++) {
		if (!isspace(query[i])) {
			return !empty;
		}
	}
	return empty;
}

/*
 * used to check if the query is correct according to the syntax of the spec
 * details
 * 	there cant be an 'and' or 'or' at the beginning or the end
 *	there cannot be an 'and' or and 'or' twice in a row
 */
bool
valid_query(char* query) 
{

	//initialize necessary variables to loop through the query
	bool valid = false;
	char* copy = malloc(strlen(query)+1);
	strcpy(copy, query);

	char split[] = " ";
	char *words[60];
	//splits the char at the next space
	char *ptr = strtok(copy, split);
	int index = 0;

	//sets an index in the words array to the next word in the query
	while (ptr != NULL) {
		words[index] = ptr;
		index++;
		ptr = strtok(NULL, split);
		
	}
	words[index] = NULL;

	//loops through the array and makes sure there are syntax issues
	//syntax issues are split up so that specific error messages can be outputted
	int i = 0;
	while (words[i] != NULL) {

		//there cant be an 'and' or an 'or' at the beginning
		if (i == 0) {
			if (strcmp(words[i], "and") == 0) {
				printf("Error: 'and' cannot be first\n");
				free(words[0]);
				return valid;
			}

			if (strcmp(words[i], "or") == 0) {
				printf("Error: 'or' cannot be first\n");
				free(words[0]);
				return valid;
			}

		}

		else {
			//there cant be an 'and' next to an 'or' or an 'and'
			if (strcmp(words[i], "and") == 0) {
			 	if(strcmp(words[i-1], "or") == 0) {
					printf("Error: 'and' and 'or' cannot be adjacent\n");
					free(words[0]);
					return valid;
				}
				else if(strcmp(words[i-1], "and") == 0) {
					printf("Error: 'and' and 'and' cannot be adjacent\n");
					free(words[0]);
					return valid;
				}
			}
			//same logic with the 'or'
			else if (strcmp(words[i], "or") == 0) {
				if (strcmp(words[i-1], "and") == 0) {
					printf("Error: 'or' and 'and' cannot be adjacent\n");
					free(words[0]);
					return valid;
				}
				else if (strcmp(words[i-1], "or") == 0) {
					printf("Error: 'or' and 'or' cannot be adjacent\n");
					free(words[0]);
					return valid;
				}
			}
		}

		//now loop through the array and make sure no char is a number
		for (int j = 0; j < strlen(words[i]); j++) {
			if (!isalpha(words[i][j])) {
				printf("Error: bad character '%c' in query\n", words[i][j]);
				free(words[0]);
				return valid;
			}
		}
		i++;
	}

	//handle for the last index
	if (strcmp(words[i-1], "and") == 0) {
		printf("Error: 'and' cannot be last\n");
		free(words[0]);
		return valid;
	}

	if (strcmp(words[i-1], "or") == 0) {
		printf("Error: 'or' cannot be last\n");
		free(words[0]);
		return valid;
	}

	//free the memory - freeing has been handled for each error message
	free(words[0]);

	return !valid;
}

/*
 * used to calculate the score for each id in the index
 * loops through the query and finds the associated counters_t struct in the index
 */
counters_t*
calculate_score(char* query, index_t* index) {

	//initialize begginning necessary variables
	char* copy = malloc(strlen(query)+1);
	strcpy(copy, query);

	char split[] = " ";
	char *words[60];
	char *ptr = strtok(copy, split);
	int pos = 0;

	while (ptr != NULL) {
		words[pos] = ptr;
		pos++;
		ptr = strtok(NULL, split);
		
	}
	words[pos] = NULL;
	int i = 0;

	//handles if the word in the array just doesnt exist in the index
	bool existing = true;

	//this counters will loop through all the words until an or is found
	counters_t* temp = counters_new();

	//this counters combines the created temp counter with the total count to handle for or
	counters_t* total = counters_new();

	//while there are still words to be checked in the array
	while (words[i] != NULL) {

		//initialize the first word into the array
		if (i == 0) {
			if (index_find(index, words[i]) != NULL) {
				counters_iterate(index_find(index, words[i]), temp, initialize);
			}
			else {
				existing = false;
			}
		}

		//if the word is not an 'and' or 'or', it is a normal word
		else if (strcmp(words[i], "and") != 0 && strcmp(words[i], "or") != 0 && existing) {

			//if it exists in the index
			if (index_find(index, words[i]) != NULL) {
				//iterate through the temp array and if that id doesnt exist in the word array
				//create it and set it to 0
				counters_iterate(temp, index_find(index, words[i]), check);
				//iteratre through the word array and use score to add it to temp
				counters_iterate(index_find(index, words[i]), temp, score);
			}

			else {
				//if it doesnt exist in the counters delete the temp and mkae a new one
				//as per the logic of 'and'
				counters_delete(temp);
				temp = counters_new();
				existing = false;
			}
		}

		//handle when there is an or
		else if (strcmp(words[i], "or") == 0) {
			//combines the 2 arrays together
			counters_iterate(temp, total, rake);

			//make a new and sequence
			counters_delete(temp);
			temp = counters_new();

			//reset existing
			existing = true;
		}
		i++;
	}

	//rake the final and sequence
	counters_iterate(temp, total, rake);
	counters_delete(temp);
	free(words[0]);

	//return the scores counters
	return total;
}

/*
 * initializes the counters passed in witht he data of the first word
 */
void
initialize(void *arg, const int key, const int count) 
{
	counters_t* temp = arg;
	//add then set it
	counters_add(temp, key);
	counters_set(temp, key, count);
}

/*
 * checks the score and sets the temps core to the lowest score between
 * the score of the temp and the passed array
 */
void
score(void *arg, const int key, const int count) 
{
	counters_t* temp = arg;
	//if the passed temp count is greater than the count iterated through
	if (counters_get(temp, key) > count) {
		//set the count in the temp array to the current iterated count
		counters_set(temp, key, count);
	}
}

/*
 * checks to see if the counter id in the temp exists in the index as well
 * if it does do nothing
 * if it doesnt add it and set the count to 0
 */
void
check(void *arg, const int key, const int count) 
{
	counters_t* indexCounter = arg;
	//if it doesnt exist in the counter of the index
	if (counters_get(indexCounter, key) == 0) {
		//place it in there with a score of 0
		counters_set(indexCounter, key, 0);
	}
}

/*
 * used to combine the temp and the total counters together to get a score
 */
void
rake(void *arg, const int key, const int count) 
{
	counters_t* total = arg;
	//iterate through the temp array and if it exists in the counter add it together
	counters_set(total, key, counters_get(total, key) + count);
}

/*
 * will display all the urls, scores, and doc ids to the user based on the search query
 * ranks the scores counters and uses that to display
 */
void
querierPrint(counters_t* scores, char* pathname)
{
	//iterate through the counters and determine the size for the rest of the method
	int size = 0;
	counters_iterate(scores, &size, sizeCalc);

	//if there is actually something in the score counters
	if (size != 0) {

		//make an array of the documents
		printf("Matches %d documents (ranked): \n", size);
		docPrint_t **ranked = calloc(size, sizeof(docPrint_t));

		//initialize every index to a NULL
		for(int i = 0; i < size; i++)  {
			ranked[i] = NULL;
		}

		//for each element in the array
		for(int i = 0; i < size; i++) {

			//instantiate a new node and set it to the index in the array
			docPrint_t* node = malloc(sizeof(docPrint_t));
			ranked[i] = node;

			//reset the count of the node for the first iteration
			node->count = 0;

			//find the highest value in the counters and set it to node
			counters_iterate(scores, node, highCalc);

			//now its in the array reset it to 0 so that it wont be grabbed again
			counters_set(scores, node->id, 0);
		}

		//time to print in each array
		for(int i = 0; i < size; i++)  {

			//open up the file and read in the url to be printed based on the id
			char* temp = malloc(strlen(pathname)+3+log10(ranked[i]->id));
			sprintf(temp, "%s/%d", pathname, ranked[i]->id);
			FILE* reader = fopen(temp, "r");
			char* url = freadwordp(reader);


			//print the data to the user
			printf("score %4d doc %4d: %s\n", ranked[i]->count, ranked[i]->id, url);

			//free the memory
			free(ranked[i]);
			free(temp);
			fclose(reader);
			free(url);
		}
		free(ranked);
	}

	//if the size is empty
	else {
		printf("No documents match\n");
	}
}

/*
 * Calculates the size of the counters
 * iterates through the array and iterates a count once
 */
void
sizeCalc(void *arg, const int key, const int count) 
{
	int *size = arg;

	if (size != NULL && count != 0) {
		(*size)++;
	}

}

/*
 * Finds the highest count in the counters structure
 * if there is a higher count, change the current high to the new struct
 */
void
highCalc(void *arg, const int key, const int count)
{
	docPrint_t *currHigh = arg;

	//if the iterated count is higher, change the current docPrint_t to the new one
	if (count > (*currHigh).count && count != 0) {
		(*currHigh).count = count;
		(*currHigh).id=key;
	}
}

static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

