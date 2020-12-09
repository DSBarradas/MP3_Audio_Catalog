/** 
 *        name: test_strset.c
 *  created on: 2020-07-02
 *      author:	Daniel Barradas        
*/ 

/* *********************  COMENTÁRIO  ********************************
 * testar na consola: 
 * 		user: dir$ make
 * 		user: dir$ test_strset < listnames.txt
 * *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strset.h"

#define MAX_NAME 30

/*=======================================================================================
Funtion Name: printHashTable

Description: 
	This fucntion prints a hashtable

Parameters: 
	strSet_t *sSet: pointer to a descriptor of a set of strings

Return:
	Doesnt have
=======================================================================================*/
static void printHashTable( strSet_t *sSet ) {
	HashListNode *entry;
	unsigned i;
	for( i = 0; i < sSet->hashsize; i++ )										// go through the hash array
		if( sSet->hashtable[i] != NULL ) {
			printf("i = %d\n", i);
		for( entry = sSet->hashtable[i]; entry != NULL; entry = entry->next )	// go through the linked list
			printf("\tname = %s\n", entry->string);
			printf("\n");
		}
				
	printf("\nNUMBER OF ENTRIES OF THE HASTABLE: %d\n\n", sSet->entries);
}


/**======================================================================================
Function Name: main
=======================================================================================*/ 
int main() {
	strSet_t *set = strSetCreate();							// create the set of string
	if( set != NULL ) {										// check if the set was successfully created
		char temp[MAX_NAME+1];
		
		while( fgets( temp, MAX_NAME+1, stdin ) != NULL ) {
			char *str = strtok(temp, "\n");					// remove the '\n' char stored by fgets
			if( str != NULL )
				strSearchAdd( set, str );	    			// insert the string in the table
		}
		
		printHashTable( set );								// pritns the hashtable
		printf("HASHSIZE: %d\n\n", set->hashsize);
		
		strSetDelete( set );								// deletes the set of string and free the mem
	}
	return 0;
}

