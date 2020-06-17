# IMPLEMENTATION.md
## Detailed pseudo code for each of the objects/components/functions

### pseudo code for the Querier.c directory (querier.c):
The main function should operate as follows:
1. parse through the arguments and set them to the necessary variables
2. Handle error testing for bad inputs
3. call the querier method and handle more functionality
4. free all the memory

The querier() function should  operate as follows:
1. While reading from stdin and user input
	1. if the `query` is not empty 
		1. normalize the query
		2. if the `normalized query` is valid
			1. print the query being used to confirm to the user
			2. calculate the score based on the query for each id and initiailize a new `counters` pointer that points to the scores that contains the id and the score for each id
			3. print out the query and the scores for each id file
			4. free the memory
		3. free the normal query
	2. print out the prompt for more querying

The is_empty() function should operate as follows:
1. loop through every char* in the `char* query`
	1. if any character isnt just a space
		1. return a true so that it is a valid query
2. return false because the array would just be spaces

The valid_query() function should operate as follows:
1. loop through the query as input
2. Separate each word into an `index` in the words array
3. Loop through each word in the array
	1. if any of the first words are 'and' or 'or' return false and print an error message
	2. if an 'and' is next to an 'and' or 'or' or an 'or' is next to an 'or' or 'and' print a specific error message and return false
	3. loop through each character in the word and return false if there is a non alpha character
4. check if the last word is an 'and' or 'or' and return false if either is true
5. free any necessary structures and return true if you havent returned anything already

The calculate_score() function should operate as follows:
1. loop through the `query` as input
2. Separate each word into an `index` in the words array
3. Initialize both counters (the `temp` which handles and sequences, and the `total` which handles or sequences)
4. Loop through each word in the array
	1. for the first word in the array, if it exists in the `index`, then add it to `temp` exactly as it is
	2. if you find a word that is NOT an 'and' or an 'or'
		1. if it exists in the `index` structure
			1. iterate through the 	`temp` counter and check if it exists in the `index` counter, if it doesnt add it
			2. iterate through the `index` counter and compare the count of the `index` counter to the count of the `temp` counter for that specific id. make the count of the `temp` counter for that specific id the lesser of the 2
		2. if it doenst exist in the `index` structure
			1. delete the `temp` counters since no id has that specific word
			2. make a new `temp` counters
	3. if you see an or in the `query`
		1. iterate through the `temp` counter and add each specific score for the id while iterating into the same id into the `total` counter
		2. delete the `temp` counters
		3. make a new `temp` in case there are more or sequences
5. after there are no more words in the `query`, iterate through the `temp` counter and add each specific score for the id while iterating into the same id into the `total` counter
6. delete the `temp` counters
7. free the necessary memory

The querierPrint() function should operate as follows:
1. iterate through the `scores` counter that was input to get the size
2. if the size is greater than 0
	1. initialize an array
	2. loop through the array and set each value to NULL
	3. loop through the array once more
		1. for each index make a new `docPrint` struct which contains an int for the file id and an id for the score
		2. set the node count to 0 so that anything will be higher
		3. iterate through the scores and find the highest score corresponding to an id
		4. set the id that was extracted from `scores` to have a count to 0
	4. loop through the array once more
		1. get the url from the `pathname` as inputted by the user by reading the first line of the document that corresponds to the id in the        `docPrint` struct that corresponds to the index in the loop
		2. print a message to the user with the id, score and url
3. else print a message that no documents match	

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

## Definition of detailed APIs, interfaces, function prototypes and their parameters

### Function prototypes & parameters for the querier.c directory:
querier.c
```
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
```

*since the function descriptions are in DESIGN.md, I will outline parameter definitions*

```int querier(char* pathname, index_t* index);``` 
- `pathname` is the directory in which the html files lie
- `index` is the data structure which contains the counts for each word for each file

`bool is_empty(char* word);` 
- `word` is the query checked to be empty or not

`counters_t* calculate_score(char* query, index_t* index);`
- `query` is the search input by the user
- `index` is the data structue which contains the counts for each word and each file

`counters_t* calculate_score(char* query, index_t* index);`
- `query` is the search input by the user
- `index` is the data structue which contains the counts for each word and each file

`void check(void *arg, const int key, const int count);`
- `arg` is the `index` counter
- `key` is the id in the `temp` counter
- `count` is the score in the `temp` counter

`void score(void *arg, const int key, const int count);`
- `arg` is the `temp` counter
- `key` is the id in the `index` counter
- `count` is the score in the `index` counter

`void rake(void *arg, const int key, const int count);`
- `arg` is the `temp` counter
- `key` is the id of the current node in the `total` counter
- `count` is the score of the current node in the `total` counter

`void initialize(void *arg, const int key, const int count);`
- `arg` is the `index` counter
- `key` is the id of the current node of the `temp` counter
- `count` is the score of the current node of the `temp` counter

`void querierPrint(counters_t* scores, char* pathname);`
- `scores` is the counters where each id pointrs to the score for that file
- `pathname` is the path wherein all the files lie

`void sizeCalc(void *arg, const int key, const int count);`
- `arg` is the count for the size
- `key` is the id of the current node in `scores`
- `count` is the id of the current node in `scores`

`void highCalc(void *arg, const int key, const int count);`
- `arg` is the new strucutre which contains the id and the count of the current highest node
- `key` is the id of the current node in `scores`
- `count` is the id of the current node in `scores`


## Data structures (e.g., `struct` names and members)

```struct docPrint_t``` used to store ```int id```, ```int count```. We nede to create a new structure so that we can pass it into the counters_iterate method.

```struct URL``` used to store the parts of the URL

```struct index_t``` used to store ```hashtable_t``` of the table, which contains a counterset `char* word`, pointing to `int id` of the file and
`int count` of the amount of times each word appears in that file

`char* query` the search input for the user

## Error handling and recovery

There are a few situations where errors must be handled for
1. In case input arguments are incorrect and not usable
2. In case any syntax of the search query is incorrect
3. In case the query is just empty

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
├── querier
│   ├── .gitignore
│   ├── DESIGN.md
│   ├── IMPLEMENTATION.md
│   ├── Makefile
│   ├── README.md
│   ├── querier.c
│   ├── testing.sh
│   └── testing.out
```
