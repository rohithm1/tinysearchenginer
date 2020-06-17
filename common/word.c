/*
 * word.c - see Implentation.md for more details
 *
 *
 * Rohith Mandavilli, April 2019.
 * 
 * Non-zero exit status if invalid input/error and 0 if success.
 * Rohith Mandavilli, April 2019, CS50
 *
 */


#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include "hashtable.h"
#include "set.h"
#include "memory.h"
#include "jhash.h"
#include "index.h"
#include "counters.h"
#include <ctype.h>

char* 
normalizeWord(char* word)
{
	//create new word to convert everything into lower case
	char* normalWord = malloc(strlen(word)+1);
	int i = 0;
	//go until the word ends
	for(; word[i] != '\0'; i++) {
		//put lowercase char into normalized word
		normalWord[i] = tolower(word[i]);
	}
	//marl the end of the lowercase word
	normalWord[i] = '\0';
	return normalWord;
}
