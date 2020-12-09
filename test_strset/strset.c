/** 
 *        name: strset.c
 *  created on: 2020-07-01
 *      author:	Daniel Barradas            
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strset.h"

/*=======================================================================================
Function Name: setInit

Description: 
	This function does a hash fucntion related to a string

Parameters:
	int hashsize: size of the hash array 
	char *key: string to code (key)
	
Return:
	unsigned int: positive integer value of the index (index of the hash array)
=======================================================================================*/
static strSet_t *setInit( strSet_t *set, int hashsize ) {
	set->hashsize = hashsize;															
	set->entries = 0;																	
	set->hashtable = calloc( set->hashsize, sizeof(HashListNode *) );
	if( set->hashtable == NULL) {
		fprintf(stderr, "\n\nERROR: COULD NOT ALLOCATE MEMORY FOR HASHTABLE \n\n");
		return NULL;
	}
	return set;
}


/**======================================================================================
Function Name: strSetCreate

Description: 
	This function creates a descriptor to a set of strings
	
Parameters:
	Doesnt have

Return:
	strSet_t *: pointer to the set of strings created
				or NULL if an error occured
	
=======================================================================================*/
strSet_t *strSetCreate( void ) {
	strSet_t *set = malloc( sizeof(strSet_t) );											// allocate memory for the of the hash table
	if( set == NULL ) {																	// check if the allocation was successfull
		fprintf(stderr, "\n\nERROR: COULD NOT ALLOCATE MEMORY FOR STRING SET \n\n");
		return NULL;
	}
		
	/* initialize the set */					
	set = setInit( set, HASH_SIZE );
	if( set == NULL )
		return NULL;
			
	return set;
}


/*=======================================================================================
Function Name: hash

Description: 
	This function does a hash fucntion related to a string

Parameters:
	int hashsize: size of the hash array 
	char *key: string to code (key)
	
Return:
	unsigned int: positive integer value of the index (index of the hash array)
=======================================================================================*/
static unsigned int hash( int hashsize, const char *key ) {
	unsigned hashval;

	for(hashval = 0; *key != '\0'; key++)				// go through the string
		hashval = *key + 31 * hashval;					// and calculate a value

	return hashval % hashsize; 							// return an index 
}


/*=======================================================================================
Function Name: reHash

Description: 
	This function does a hash fucntion related to a string

Parameters:
	int hashsize: size of the hash array 
	char *key: string to code (key)
	
Return:
	unsigned int: positive integer value of the index (index of the hash array)
=======================================================================================*/
static void reHash( strSet_t *set ){
	HashListNode **oldHashtable, *oldEntry;
	int oldHashsize, hashval;
	unsigned i;
	
	oldHashtable = set->hashtable;
	oldHashsize = set->hashsize;
	
	/* create the new hashtable and rehash the old entries */
	if( setInit( set, oldHashsize*2+1 ) != NULL ) {
		for( i = 0; i < oldHashsize; i++ ){
			oldEntry = oldHashtable[i];
			
			while( oldEntry != NULL ){
				HashListNode *temp;
	
				hashval = hash( set->hashsize, oldEntry->string );			
				temp = oldEntry->next;
				
				/* head insertion */
				oldEntry->next = set->hashtable[hashval];					// the next of the new entry points to the first elem of the list or NULL in case it no entry exists yet
				set->hashtable[hashval] = oldEntry;					// the new entry points to the first entry of that list	
				
				set->entries++;
				oldEntry = temp;
			}
		}
	}
	
	free( oldHashtable );
	
	return;
}



/*=======================================================================================
Function Name: lookUp

Description: 
	This function searches for a string in the hashtable
	
Parameters:
	strSet_t *set: pointer to the set of strings
	const char *key: string for the search

Return:
	HashListNode *: NULL if the string doesnt exist; 
					pointer to the struct that contains the string
	
=======================================================================================*/
static HashListNode *lookUp( strSet_t *set, const char *key ) {
	HashListNode *entry;

	for( entry = set->hashtable[hash(set->hashsize,key)]; entry != NULL; entry = entry->next ) 	// go through the linked list
		if( strcmp(key, entry->string ) == 0)														// chek if the key exists
			return entry;																				// return pointer to that entry
									
	return NULL;																				// otherwise, the NULL pointer indicates the key wasnt found  
}


/**======================================================================================
Function Name: strSearchAdd

Description: 
	This fucntion searches for a string in a set of strings. If it exists does nothing
	and if it doesnt exist it creates an entry of that string
	

Parameters:
	strSet_t *: pointer to the set of strings
	const char *str: pointer to a const string to be searched in the set

Return:
	char *: pointer to a string that existed previously or was allocated in this call
			of the fucntion, or NULL if an error ocurred
=======================================================================================*/
char *strSearchAdd( strSet_t *set, const char *str ) {
	HashListNode *entry;
	unsigned hashval;
	
	/* expand the table if needed */
	while( set->entries >= LOAD_FACTOR * set->hashsize )
		reHash( set );
	
	/* insert the new entry */
	if( (entry = lookUp(set,str)) == NULL ) {				// check if the string already exists
		entry = malloc( sizeof(HashListNode) );			// if it doesnt exist, allocante memory for a new entry
		if(entry == NULL)
			return NULL;
	
		entry->string = strdup(str);
		if( entry->string == NULL ) {
			free( entry );
			return NULL;
		}
		
		hashval = hash(set->hashsize, str);						// generates a the index referent to the string
			
		/* head insertion */
		entry->next = set->hashtable[hashval];					// the next of the new entry points to the first elem of the list or NULL in case it no entry exists yet
		set->hashtable[hashval] = entry;						// the new entry points to the first entry of that list	
		
		set->entries++;
	}
	
	return entry->string;								// if it saved the string, returns the entry created 
}


/*=======================================================================================
Function Name: freeHashList

Description:
	This fucntion frees a linked list of a bucket of the hash array.

Parameters:
	HashListNode *entry: pointer to the head (first element of the list) 

Return:
	Doesnt have
=======================================================================================*/
static void freeHashList( HashListNode *entry ) {
	if( entry == NULL )
		return;
	if(entry->next != NULL)	{			// checks if there are more entries and if positive frees them
		freeHashList( entry->next );		
	}
	
	free( entry->string );				// frees the eentry's string
	free( entry );						// frees the entry passed to the fucntion
}


/**======================================================================================
Function Name: strSetDelete

Description: 
	This fucntion deletes the set of strings and frees the mem referent to that
	set and to the strings of that set

Parameters:
	strSet_t *: pointer to the string set to be freed 

Return:
	Doesnt have
=======================================================================================*/
void strSetDelete( strSet_t *set ) {
	HashListNode *entry;
	
	unsigned i;
	for( i = 0; i < set->hashsize; i++ ) {		// go through the hasharray
		entry = set->hashtable[i];
		freeHashList( entry );					// frees the linked list of a bucket of the hasharray
	}
	
	if( set->hashtable != NULL )
		free( set->hashtable );					// frees the hasharray
	
	free( set );
}
