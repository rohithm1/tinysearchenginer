# User interface

## Querier.c
Executed through Command Line Interface (CLI):

`./querier pageDirectory indexFilename`

# Inputs and Outputs

## Querier.c
### Inputs
need `pagedirectory name` and the `indexFilename`
1. where `pageDirectory` is the pathname of a directory produced by the Crawler or relevant test files locations, and
2. where `indexFilename` is the pathname of a file in which indexer.c created to store the relevant counts of each word for each file id.

### Outputs
The Querier should output to the user the ranked list of urls based on the search query
Must be in this format:
```
$ querier/querier data/toscrape-depth-2 data/toscrape-index-2 
Query? Europe travel
Query: europe travel 
Matches 8 documents (ranked):
score   3 doc 575: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/neither-here-nor-there-travels-in-europe_198/index.html
score   1 doc  91: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-travelers_285/index.html
score   1 doc 573: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/travel_2/index.html
score   1 doc 574: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/1000-places-to-see-before-you-die_1/index.html
score   1 doc 576: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-road-to-little-dribbling-adventures-of-an-american-in-britain-notes-from-a-small-island-2_277/index.html
score   1 doc 577: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/a-year-in-provence-provence-1_421/index.html
score   1 doc 578: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-great-railway-bazaar_446/index.html
score   1 doc 579: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/a-summer-in-europe_458/index.html
-----------------------------------------------
```

# Functional decomposition into modules
Anticipating the following methods/functions:
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
int querier(char* pathname, index_t* index): helper function to actually handle the querying

bool is_empty(char* word): helper function to check if a query is empty or not

bool valid_query(char* query): helper function to check if the input query is valid according to the following syntax:
	- combines words with two operators: and, or.
	- Each operator is left-associative, just like multiply and add
		- The and operator takes precedence over the or operator, just like multiply takes precedence over add.
	- Although the syntax allows and to be omitted, you should mentally insert an and wherever an operator is missing

counters_t* calculate_score(char* query, index_t* index): returns the counters of scores with each id and count to be printed

void score(void* arg, const int key, const int count): calculates the score for each id (deals with the and operator)

void rake(void * arg, const int key, const int count): combines the and sequences together to make the total score for each id

void initialize(void * arg, const int key, const int count): gets the first counters in the index and initializes it to the temp counter and sequences so that the counters struct has the beginning ids fo the files it needs

void querierPrint(counters_t* scores, char* pathname): prints the query with the document urls scores and ids

void sizeCalc(void * arg, const int key, const int count): used to find the size of the counters for the array being created

void check(void * arg, const int key, const int count): makes sure the file ids exist in both arrays, if not it adds them to it

void highCalc(void * arg, const int key, const int count): finds the highest value in the scores counters to be added to the array

static void prompt(void): prints out "query?" for the user

int fileno(FILE * stream): prevents a compilation warning for the prompt() method

# Pseudo code for logic/algorithmic flow
## Querier.c
### Main
1. Takes input through command line
2. Validate the command line arguments
3. Set variables to each input and modify as necessary so that they are usable
4. Call Querier()

### Querier()
1. Read from stdin
2. Validate query
3. if validated calculate the score for each word in the query
4. print the quert

### valid_query()
1. Split the query into individual words into an array
2. check for syntax errors within the query
3. if there are syntax errors found, display a specific message and return false, otherwise return true

### calculate_score()
1. separate the query into an array of different words to calculate the score for each one
2. instantiate 2 counters structs to hold and sequence data and or sequence data
3. loop through all the words passed in
4. initialize the and sequence counter with the first word in the array
5. for each word that is not an 'and' or an 'or', use and squence logic to replace the score of the and sequence counters
6. if you encounter an 'or', rake the temp counters struct into the total structure
7. return the total scores to the querier method for printing

### querierPrint()
1. find the total size of the scores counters struct
2. if it isnt empty, start the process of printing by making an array
3. make a new structure that contains 2 ints each representing the id of a file and its respective score
4. iterate through the counters and find the highest score id, setting the values to the passed through new data structure
5. as you iterate, if there is a higher value, change what the structure is equal to
6. after iterating, set the score of the id of the highest score in the scores counters to 0 so that it isnt read again
7. loop through the array created (which should contain a list in order of the ids and the counts) and print out to the user the list of ranked urls based on the output provided above

# Dataflow through modules

## Querier.c
main() passes the index into querier() and the pathname used to get the url and therefore print the url
querier() calls valid_query() (passing it the query read in from stdin), calculate_score() (passing in the query that has been normalized and validated) and querierPrint() (passing in the scores counter structure that contains each file id and the respective score it has)

calculate_score sometimes adds data to the index. It will add a file id and the count it has to the index coutners for that respective wordid and opnly if it exists in the temp counters and if and only if the count for it is 0 only for the purpose of not forgetting any fiels that exist in the temp and not in the data the index holds.

querierPrint edits the counters it is passed through by setting some of the id counts to 0 after theu have been extracted - it shouldnt affect anything since the data has already been handled for

# Major data structures
A data structure that contains 2 integers must be created so that it can be passed and set data to by the querierPrint method. 

# Testing plan
1. Check a multiplicity of different input intentionally made to cause errors
2. check user input in terms of pathnames and such
3. according to the requirements spec, check things like bad characters, incorrect syntax
	- within that it is necessary to make sure each error has a specific error message
4. also check complex and random inputs so that we can assure our program is versatile




