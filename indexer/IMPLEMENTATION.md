# IMPLEMENTATION.md
## Detailed pseudo code for each of the objects/components/functions

### pseudo code for the Indexer.c directory (indexer.c):
The main function should operate as follows:
1. parse through the arguments and set them to the necessary variables
2. Handle error testing for bad inputs
3. call the indexer method and handle more functionality

The Indexer function should operate as follows:
1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. Create a new `FILE* file` based on the command line input
4. Instantiate the `index_t* index` as an empty data structure
	1. must specify the amount of slots the `hashtable_t* table` will hold (shall be between 200 and 900)
5. Begin calling the new functions
6. if the exit status is still 0,
	1. Call index_build() from the index.c module - passes `pathname` and `index`
	2. if that method call is successful, call index_save().
7. delete the index and return the exit status to the main method

### pseudo code for the Indextest.c directory (indextest.c):
The main function should operate as follows:
1. parse through the arguments and set them to the necessary variables
2. Handle error testing for bad inputs
3. Instantiate the `index_t* index` as an empty data structure
	1. must specify the amount of slots the `hashtable_t* table` will hold (shall be between 200 and 900)
4. If the first file exists
	1. load the inverted index based on the filename data
	2. if the second file exists
		1. call index_save() to print to the file
	3. else print error
5. else print error
6. delete index


### pseudo code for the common directory (index.c):
*NOTE: The specific pseudocode for the functions like index_new(), index_insert(), etc. is not provided because of their similarities to the hashtable.c files. For more information please look at the previous modules created and us that as a guide for some index.c functions.*

The index_build() function should operate as follows:
1. set the `int id` variable to 1
2. create a temporary pathname to the first file in the crawler directory using the `int id`
3. while there are still filenames to read in the crawler directory
	1. read the first 3 lines into `char *` variables for URL, and HTML, and then `int depth` for the depth of the page
	2. create a new `webpage_t* page` with the read variables
	3. While there are still words to read in the html of the `webpage_t* page`
		1. normalize the word into `char *` (see word.c for more details)
		2. if the normalized word length is above 3
			1. if the normalized word is already in the data structure
				1. find the `counters_t *` associated with the word
				2. increment the counter
			2. else if it is not already in the structure
				1. Create a new `counters_t *` variable
				2. insert a new set with the `int id` of the file
				3. Insert the `counters_t *` variable into the data structure
		3. Free the memory used (`char* word` and `char* normalWord`)
	4. increment the id
	5. create a new pathname
	6. free any memory
	7. delete the webpage
4. free any more data structures

The index_save() function should operate as follows:
1. If the inputs are not NULL and usab;e
	1. iterate through the hashtable and call hashtable_func
		*hashtable func will print the word associated and call counters_func which prints the id of the file and count of the word*
	2. return 0 after closing the file
3. else return 1

The index_load() function should operate as follows:
1. Instantiate a new word to be used to scan the file
2. While there are still words to scan
	1. create a new `counters_t* ` variable
	2. insert the `counters_t *` into the data structure
	3. while there are still numbers to scan in the line
		1. set the counters of that word we just read to contain the `int id` and `int count` of the file
	4. free the word and remalloc the word
3. free the word - handles the last instance of the word
4. close the file

### pseudo code for the common directory (word.c):

The normalizeWord() function should operate as follows:
1. malloc a new `char* normalWord` with the same size as the `char * word` input
2. loop through each `char` in the `word` array and set the corresponding `char` in the `normalWord` array to a lowercase version of the `char` in `word` using the tolower() function.
3. set the last char in normalWord to a terminating character ('\0')
4. return the `normalWord`

## Definition of detailed APIs, interfaces, function prototypes and their parameters

### Function prototypes & parameters for the indexer.c directory:
indexer.c
```
int main(int argc, char *argv[]);
int indexer(char* pathname, char* filename);
```
```int main(int argc, char *argv[]);``` main checks input parameters nad validates them

```int indexer(char* pathname, char* filename);``` indexer calls build and save to fill data structure and output to file

indextest.c
`int main(int argc, char *argv[]);` checks input parameters, validates them, calls index_load, and index_save

index.c
```
index_t * index_new(const int numslots);
counters_t* index_find(index_t* index, char* word);
bool index_insert(index_t* index, char* word, counters_t* counter);
int index_save(FILE* file, index_t* index);
void hashtable_func(void *arg, const char *key, void *item);
void counters_func(void *arg, const int key, const int count);
void index_delete(index_t* index);
void itemdelete(void *item);
int index_build(char* pathname, index_t* index);
int index_load(FILE* file, index_t* index);
```

`index_t * index_new(const int numslots);` creates the new index - calls hashtable_new and passes the numslots into it

`counters_t* index_find(index_t* index, char* word);` attempts to find the word in the input index and returns the `counters_t* ` variable assocaited 
with it

`bool index_insert(index_t* index, char* word, counters_t* counter);` inserts the `word` and `counter` into the `index`

`int index_save(FILE* file, index_t* index);` prints the `index` data structure in to the `file` input with specific format to be read later

`void hashtable_func(void *arg, const char *key, void *item);` is the printing function passed to index_save - prints the word associated in the 
hashtable

`void counters_func(void *arg, const int key, const int count);` is the printing function called by hashtable_print, prints the id and the word count 
associated with that file

`void index_delete(index_t* index);` deletes the `index` by calling hashtable_delete, and will also free the `index` passed through

`void itemdelete(void *item);` used to free the item passed through

`int index_build(char* pathname, index_t* index);` reads the files in the directory `pathname` and parses them into the `index` - called in the indexer.c file

`int index_load(FILE* file, index_t* index);` reads the file passed through and parses the data into `index` - called in the indextest.c file


## Data structures (e.g., `struct` names and members)

```struct webpage webpage_t``` used to store ```char* url```, ```char* html```, ```int depth```, and ```size_t html_len```.

```struct URL``` used to store the parts of the URL

```struct index_t``` used to store ```hashtable_t``` of the table, which contains a counterset `char* word`, pointing to `int id` of the file and
`int count` of the amount of times each word appears in that file

## Error handling and recovery

There are a few situations where errors must be handled for
1. In case input arguments are incorrect and not usable
2. In case in any situation files are opened incorrectly
3. If the index.c module returns any errors

## Resource management

1. Need to make sure all memory that is created using Malloc, there is a free that gets rid of them.
2. Code handles for malloced memory in private methods as well as memory malloced by the actual code itself.
3. Also handles for memory allocation given boundary error cases
4. Also deletes any data structures originally created

## Files

```
├── .gitignore
├── Makefile
├── README.md
├── common			# added by you (Lab4)
│   ├── .gitignore
│   ├── Makefile
│   ├── README.md
│   ├── index.c		# added by you (Lab5)
│   ├── index.h		# added by you (Lab5)
│   ├── pagedir.c
│   ├── pagedir.h
│   ├── word.c		# added by you (Lab5)
│   └── word.h		# added by you (Lab5)
├── crawler			# added by you (Lab4)
│   ├── .gitignore
│   ├── IMPLEMENTATION.md
│   ├── Makefile
│   ├── README.md
│   ├── testing.sh
│   ├── testing.out
│   └── crawler.c
├── indexer			# added by you (Lab5)
│   ├── .gitignore
│   ├── DESIGN.md
│   ├── IMPLEMENTATION.md
│   ├── Makefile
│   ├── README.md
│   ├── indexer.c
│   ├── indextest.c
│   ├── testing.sh
│   └── testing.out
├── libcs50
│   ├── .gitignore
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
