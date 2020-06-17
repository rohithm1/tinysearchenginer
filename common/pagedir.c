/*
 * pagedir.c - contains the functions common to crawler, indexer and quierier
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

/* Private function prototypes */
bool checkName(char* filename);


bool
checkName(char* filename)
{
	FILE* file;
	if ((file = fopen(filename, "w")) == NULL) {
		return false;
	}
	else {
		fclose(file);
		return true;
	}
}



