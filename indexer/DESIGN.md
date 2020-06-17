# User interface

## Indexer.c
Executed through Command Line Interface (CLI):

`./indexer pageDirectory indexFilename`

##Indextest.c
Executed through Command Line Interface (CLI):

`./indextest filename1 filename2`

# Inputs and Outputs

## Indexer.c
### Inputs
need `pagedirectory name` and the `indexFilename`
1. where `pageDirectory` is the pathname of a directory produced by the Crawler or relevant test files locations, and
2. where `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

### Outputs
The Indexer should create a file based on the inverted darta structure in the program. The file must follow the format:
1. one line per word, one word per line
2. each line provides the word and one or more (docID, count) pairs, in the format
3. word docID count [docID count]...
4. where word is a string of lower-case letters,
5. where docID is a positive non-zero integer,
6. where count is a positive non-zero integer,
7. where the word and integers are separated by spaces

## Indextest.c

### Inputs
need `filename1` and `filename2`
1. where `filename1` is the filename of the output produced by indexer.c
2. where `filename2` is the new file that indextest.c will output to

### Outputs
The indextest will output a file with the exact same format as indexer - the point of the program is to test if indexer.c correctly works
The file produced should be the same as the file read in.
The File will follow the following rules
1. one line per word, one word per line
2. each line provides the word and one or more (docID, count) pairs, in the format
3. word docID count [docID count]...
4. where word is a string of lower-case letters,
5. where docID is a positive non-zero integer,
6. where count is a positive non-zero integer,
7. where the word and integers are separated by spaces

# Functional decomposition into modules



# Pseudo code for logic/algorithmic flow
## Indexer.c
### Main
1. Takes input through command line
2. Validate the command line arguments
3. Set variables to each input and modify as necessary so that they are usable
4. Call Indexer()

### Indexer()
1. Create the file and open it as writable - check if its null
2. Instantiate the index
3. Call index_build() and index_save() if the exit status is still 0 
	build and save will be in the index.c module which is described in the implementation.md file

## Indextest.c
### Main
1. Validate command line parameters/instantiate any necessary variables
2. Call index_build()
3. Call index_save()

# Dataflow through modules

## Indexer.c
Indexer() calls both index_build() and index_save() in the index.c module and passes the index instantiated in indexer.c for both functions
index_build() edits and modifies the data structure passed through, while the save function uses the data to print to a file passed through based on the format specified in the requirements spec

## Indextest.c
the main function in indextest.c calls index_load() which reads the file created by the crawler (also inputted in the command line by the user who calls indextest)
then it calls index_save() again which saves the newly filled data structure index and writes it to a file

# Major data structures
An inverted-index data structure maps words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. 
`hashtable_t` that takes the word as its key and has a `counter` as its item. The `int key` of the counter will be the unique document id name, and the `int value` will be the amount of times the word (`char* key` of the hashtable) appears in the HTML file.
More documentation provided in the IMPLMENTATION.md file for specific methods

# Testing plan
## Testing plan/formal should generally work for both indexer.c and indextest.c
1. Find a small, "internal", web page and make the console display the word count for each word, then work through and make sure the counts are accurate.
2. Check through data structure to guarantee the index filename was read correctly into major data structures.
3. Will check to make sure the amount of arguments are correct, make sure the files are able to be easily opened.
4. NO erroneous error checking is necessary since we will assume that the content of the index file is correct as specified.
5. check if the files are also writable for both of them
5. make sure normal command lines demonstrate functionality




