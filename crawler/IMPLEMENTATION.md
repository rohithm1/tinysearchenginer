## Detailed pseudo code for each of the objects/components/functions

### pseudo code for the Crawler directory (crawler.c):
The main function should operate as follows:
1. parse through the arguments and set them to the necessary variables
2. Handle error testing for bad inputs
3. call the crawler method and handle more functionality

According to the Design Spec, the Crawler function should operate as follows:
1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. make a webpage for the ```seed```, marked with depth=0
4. add that page to the bag of webpages to crawl
5. add that URL to the hashtable of URLs seen
6. while there are more webpages to crawl,
	1. extract a webpage (URL,depth) item from the bag of webpages to be crawled,
	2. pause for at least one second,
	3. use pagefetcher to retrieve a webpage for that URL,
	4. use pagesaver to write the webpage to the ```pathname``` with a unique document ID, as described in the Requirements.
	5. if the webpage depth is < ```maxDepth```, explore the webpage to find links:
		1. use pagescanner to parse the webpage to extract all its embedded URLs;
		2. for each extracted URL,
			1. ‘normalize’ the URL (see below)
			2. if that URL is not ‘internal’ (see below), ignore it;
			3. try to insert that URL into the hashtable of URLs seen
				1. if it was already in the table, do nothing;
				2. if it was added to the table,
					1. make a new webpage for that URL, at depth+1
					2. add the new webpage to the bag of webpages to be crawled

The pagefetcher function should operate as follows:
1. Attempt to call ```webpage_fetch()``` and print to stdout the result of the function

The pagescanner function should operate as follows:
1. while looping through each URL in the HTML text
	1. print to console that each URL has been found
	2. if the URL found is "internal"
		1. if the result has been inserted to the hashtable correctly
			1. add the page to the bag
		2. else free the memory used for the URL
	3. else print the URL has been ignored and free the memory used

### pseudo code for the common directory (pagedir.c):

The nameCheck function should operate as follows:
1. Check if the ```char* filename``` input is a writable file
2. if it isnt
	1. return false
3. if it is
	1. return true

The pagesaver function should operate as follows:
1. Create the stable ```pathname``` with the correct id
2. attempt to open the file
3. print the HTML text (retrieved from ```page```) to the file opened in ```pathname```


## Definition of detailed APIs, interfaces, function prototypes and their parameters

### Function prototypes & parameters for the Crawler directory:
Crawler.c
```
void itemdelete(void *item);
void URLdelete(void *item);
int crawl(webpage_t* seed, char* pathname, int depth, char* url);
void pagefetcher(webpage_t* page);
void pagescanner(webpage_t* page, hashtable_t* table, int depth, bag_t* bag);
inline static void logr(const char *word, const int depth, const char *url);
```
```itemdelete(void *item)``` is a function passed in used to free the items in the bag used to store pages

```void URLdelete(void *item)``` is the function passed for hashtable. Basically only symbolic - contains no code because each value in the key-value pair in the hashtable is 
an empty string, meaning there is nothing to free

```int crawl(webpage_t* seed, char* pathname, int depth, char* url)``` returns 0 if works correctly, false if any error happens. Handles crawling through webpages, see 
psuedocode for more details.

```void pagefetcher(webpage_t* page)``` is the function that fetches the HTML of each ```page``` in the bag. It will save the HTML string to a variable in the page 
```struct```

```void pagescanner(webpage_t* page, hashtable_t* table, int depth, bag_t* bag)``` is the function that scans the HTML from the given ```page``` and finds more URLS 
to search, 
adding it to the ```bag_t* bag```.

### Function prototypes & parameters for the Common directory:
pagedir.c (note this is incomplete since the next labs will add onto the code in pagedir)
```
int pagesaver(int id, char *pathname, webpage_t* page);
bool checkName(char* filename);
```
```int pagesaver(int id, char *pathname, webpage_t* page);``` is a function that writes the details of a url and its html to a specific file with a unique ```filename```

```bool checkName(char* filename);``` is a function that checks if the input ```char* filename``` exists.

## Data structures (e.g., `struct` names and members)

```struct webpage webpage_t``` used to store ```char* url```, ```char* html```, ```int depth```, and ```size_t html_len```.

```struct URL``` used to store the parts of the URL

```struct bag``` used to store the ```struct bagnode *head``` of the list

```struct hashtable_t``` used to store ```unsigned long size``` of the table, and the ```set_t **table``` which has the list of sets in the table.

## Error handling and recovery

There are a few situations where errors must be handled for
1. In case input arguments are incorrect and not usable
2. In case in any situation files are opened incorrectly

## Resource management

1. Need to make sure all memory that is created using Malloc, there is a free that gets rid of them.
2. Code handles for malloced memory in private methods as well as memory malloced by the actual code itself.
3. Also handles for memory allocation given boundary error cases

## Files

```
├── Makefile
├── README.md
├── common			# added by you
│   ├── Makefile
│   ├── pagedir.c
│   └── pagedir.h
├── crawler			# added by you
│   ├── IMPLEMENTATION.md
│   ├── Makefile
│   ├── README.md
│   ├── testing.sh
│   ├── testing.out
│   └── crawler.c
├── libcs50
│   ├── Makefile
│   ├── README.md
│   ├── bag.c
│   ├── bag.h
│   ├── counters.c	# if you decided to add your Lab3 solution
│   ├── counters.h
│   ├── file.c
│   ├── file.h
│   ├── file.md
│   ├── hashtable.c	# if you decided to add your Lab3 solution
│   ├── hashtable.h
│   ├── jhash.c
│   ├── jhash.h
│   ├── libcs50-given.a
│   ├── memory.c
│   ├── memory.h
│   ├── memory.md
│   ├── set.c		# if you decided to add your Lab3 solution
│   ├── set.h
│   ├── webpage.c
│   ├── webpage.h
│   └── webpage.md
```
