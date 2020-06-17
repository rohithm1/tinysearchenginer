/*
 * pagedir.h - contains the function descriptions common to crawler, indexer and quierier
 *
 * Rohith Mandavilli, April 2019, CS50
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include <string.h>
#include <math.h>


/* shared functions */

/*
 * checks if the filename input is a directory/file existing
 *
 * returns true if it does exist
 * returns false if doesnt
 */
bool checkName(char* filename);


