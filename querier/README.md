# Rohith Mandavilli
## CS50 Spring 2019, Tiny Search Engine, querier directory

GitHub username: rohithm1

To build, run `make` - will run querier.c

To test, using my script, run `make test` which will also call the crawler files and indexer files to make sure they are updated

To clean up, run `make clean`.

to successfully test the file everything needs to be made in tse
the testing script will produce output files that were not pushed to git, and if you want those output files to be deleted, run `make clean`

With regards to functionality, my code supports 'and' precedence over 'or' and prints the documents in decreasing order by score, meeting the full spec. To clarify, I am hoping to recieve full score on functionality.

