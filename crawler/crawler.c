/*
 * crawler.c - see Implentation.md for more details
 *
 * usage: ./crawler [URL] [directory name] [depth]
 * 	URL must be 'internal'
 *	directory name must be a valid directory
 *	depth must be greater than 0
 * stdin: stdin is not used
 * stdout: all calculations are printed to stdout in the form of a table with details 
 *	about how each link was analyzed.
 *
 * Rohith Mandavilli, April 2019.
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
#include "hashtable.h"
#include "pagedir.h"
#include <string.h>
#include <math.h>


/*  function prototypes */
void itemdelete(void *item);
bool pagefetcher(webpage_t* page);
int crawl(webpage_t* seed, char* pathname, int depth, char* url);
void pagescanner(webpage_t* page, hashtable_t* table, int depth, bag_t* bag);
void URLdelete(void *item);
inline static void logr(const char *word, const int depth, const char *url);
int pagesaver(int id, char *pathname, webpage_t* page);

int
main(int argc, char *argv[])
{
	//initialize all arguments into function variables
	const int depth = atoi(argv[3]);

	//set memory area for URL
	char *url = (char*)malloc(1 + strlen(argv[1])*sizeof(char));
	strcpy(url, argv[1]);
	int exit = 0;

	//set memory area for the directory name
	char *pathname = (char*)malloc(1 + strlen(argv[2])*sizeof(char));
	strcpy(pathname, argv[2]);

	//set memory area for the filename
	char *filename = (char*)malloc(10 + strlen(argv[2])*sizeof(char));
	strcpy(filename, argv[2]);
	strcat(filename, "/.crawler");

	//error testing
	if (argc != 4) { 
		printf("Incorrect arg amount\n");
		exit = 1;
	}

	if (depth < 0 || depth > 10) {
		printf("Maxdepth is below 0 or greater than 10\n");
		exit = 1;
	}

	if (!IsInternalURL(url)) {
		printf("URL is not valid/'internal' to the domain\n");
		exit = 1;
	}

	if (!checkName(filename)) {
		printf("Pathname doesn't exist\n");
		exit = 1;
	}
	
	//initialize the first webpage
	webpage_t* seed = webpage_new(url, 0, NULL);
	
	//only crawl if there already hasnt been an error
	if (exit == 0) {
		exit = crawl(seed, pathname, depth, url);
	}
	else {
		webpage_delete(seed);	//need to free data that wouldve been freed in crawl
	}

	free(pathname);
	free(filename);

	return exit;
}

/*
 * Meat of the program - actually handles crawling through the pages
 *
 */
int
crawl(webpage_t* seed, char* pathname, int depth, char* url)
{
	//create data structures to use in function
	bag_t* bag;
	hashtable_t* table;
	webpage_t* page;

	bag = bag_new();
	table = hashtable_new(100);

	//insert initial page to start crawl
	bag_insert(bag, seed);
	hashtable_insert(table, url, seed);

	int id = 1;

	//while there are still things to crawl
	while ((page = bag_extract(bag)) != NULL) {

		//get the HTML of the page
		if (pagefetcher(page)) {

			//save it to a file
			if (pagesaver(id, pathname, page) == 1) {
				return 1;
			}
			id++;

			//if the current depth is still less than the max, continue
			if (webpage_getDepth(page) < depth) {
				logr("Scanning", webpage_getDepth(page), webpage_getURL(page));

				//perform adding to the hashtable
				pagescanner(page, table, webpage_getDepth(page), bag);
			}
		}
		
		webpage_delete(page);
	}
	bag_delete(bag, itemdelete);
	hashtable_delete(table, URLdelete); //segfault
	return 0;
}

/*
 * Need to use function to pass for bag_delete
 * will free the item in bag passed in
 *
 */
void 
itemdelete(void *item)
{
	if (item != NULL) {
    	webpage_delete(item);  
 	}
}

/* 
 * need empty method for empty string inserted for hashtable_delete
 */
void
URLdelete(void *item){ }

/*
 * Function gets the HTML of each page and stores it in the data structure
 * Will print an error message if there was a fetch failure
 * true if successful, false if not
 *
 */
bool
pagefetcher(webpage_t* page)
{
	if (webpage_fetch(page)) {
		logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
		return true;
	}
	else {
		printf("HTML fetch failure for URL %s!\n", webpage_getURL(page));
		return false;
	}
}

/*
 * Loops through each HTML and finds all the URL's
 * prints to user action with each URL
 * will only add URLs to the table that are not duplicate and 'internal'
 */
void
pagescanner(webpage_t* page, hashtable_t* table, int depth, bag_t* bag)
{
	char* result;
	int pos = 0;
	//find each URL in the HTML
	while ((result = webpage_getNextURL(page, &pos)) != NULL) {
		logr("Found", depth, result);

		//check if internal
		if (IsInternalURL(result)) {

			//insert into table and bag if unique
			if (hashtable_insert(table, result, " ")) {
				webpage_t* temp;
				temp = webpage_new(result, depth +1, NULL);
				bag_insert(bag, temp);
			}

			//else ignore duplicates and free data
			else {
				logr("IgnDup", depth, result);
				free(result);
			}
		}

		//else ignore external and free data
		else {
			logr("IgnExtrn", depth, result);
			free(result);
		}

	}
}

/*
 * prints the HTML and other file details to a specific file
 * returns 0 if successful
 * returns 1 if fails
 */
int
pagesaver(int id, char *pathname, webpage_t* page)
{
	//get the right filename in the form of a string
	int numDigits = floor(log10(abs(id))) + 1;
	char filename[numDigits +strlen(pathname)];
	sprintf(filename, "%s/%d", pathname, id);

	FILE* file;

	//check if the file was created correctly
	if ((file = fopen(filename, "w")) == NULL)  {
		return 1;
    }

    //print to the file
    fprintf(file, "%s\n", webpage_getURL(page));
	fprintf(file, "%d\n", webpage_getDepth(page));
	const char* html = webpage_getHTML(page);
	fprintf(file, "%s\n", html);
	fclose(file);
	return 0;
}

//printing... given by Professor Kotz
inline static void 
logr(const char *word, const int depth, const char *url)
{
  printf("%2d %s%9s: %s\n", depth, "", word, url);
}

