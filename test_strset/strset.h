/** 
 *        name: strset.h
 *  created on: 2020-07-01
 *      author:	Daniel Barradas           
*/ 
#ifndef STRSET_H
#define STRSET_H

#define HASH_SIZE 51
#define LOAD_FACTOR 0.7
 
typedef struct hashListNode {
	struct hashListNode *next;		// pointer to the next element of the list
	char *string;						// pointer to a string in dynamic memory
} HashListNode;						/* linekd list type that can exist in each elem of the hash array (hash entry) */ 

typedef struct {
	int hashsize;					// size of the hash array
	int entries;					// number of entries of the hash array  
	HashListNode **hashtable;		// hash array
} strSet_t;							/* type that has an hashtable info */

strSet_t *strSetCreate( void );

char *strSearchAdd( strSet_t *set, const char *str );

void strSetDelete( strSet_t *set );

#endif
