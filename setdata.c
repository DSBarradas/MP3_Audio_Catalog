/** 
 *        name: setdata.c
 *  created on: 2020-05-24
 *      author:	Daniel Barradas	           
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtype.h"
#include "tag.h"
#include "binarytree.h"
#include "setdata.h"

/**======================================================================================
Funtion Name: setCreate

Description: 
	This fucntion creates the set of tag's descriptor in an inicializes its members

Parameters:
	Doesnt have

Return:
	TagSet_t *: pointer to the set descriptor created
=======================================================================================*/
TagSet_t *setCreate( void ) {
	TagSet_t *set = malloc( sizeof(TagSet_t) );		// allocate memory for the of the set of tags descriptor
	if( set == NULL ) {								// check if the allocation was successfull
		fprintf(stderr, "\n\nERROR: COULD NOT ALLOCATE MEMORY FOR TAG SET \n\n");
		return NULL;
	}
		
	/* initialize the set descriptor */					
	set->size = 0;
	set->count = 0;
	set->data = NULL;
	set->aux = NULL;
			
	return set;
}

/**======================================================================================
Function Name: setAdd

Description: 
	This function adds a tag, pointed by tag, to the descriptor, pointed by set

Parameters:
	TagSet_t *set: pointer to a set of tags descriptor
	MP3Tag_t *tag: pointer to a tag

Return:
	Doesnt have
=======================================================================================*/ 
void setAdd( TagSet_t *set, MP3Tag_t *tag ) {
	if( set != NULL ){															// check if the set exists
		set->data = realloc( set->data, sizeof(MP3Tag_t *) * (set->size+1) );	// allocate memory for the array of pointers to the structs MP3Tag_t
		if( set->data != NULL ) {												// check if the allocation was successfull
			if( tag != NULL ) {													// check if the tag exists
				set->data[set->count] = tag;						// save the pointer to the MP3Tag_t with the tag info in the array pointed by data
				set->count++;										// incremment the number of pointers to the tags successfully stored
			}
			set->size++;											// incremment the number of elemts allocated in the array pointed by data 
		}
	}
}


/*=======================================================================================
Function Name: compareTitle

Description: 
	This function, used by qsort, compares the titles, then the artists (if the
	titles are the same) and finally the albums (if the artists are the same)

Parameters:
	const MP3Tag_t **tag1: pointer to a tag pointer
	const MP3Tag_t **tag2: pointer to another tag pointer

Return:
	int: (comparsion result)	>0 (**tag1 > **tag2)
								<0 (**tag1 < **tag2)
								=0 (**tag1 == **tag2)
=======================================================================================*/ 
static int compareTitle( const MP3Tag_t **tag1, const MP3Tag_t **tag2 ) { 
	if( strcmp( (*tag1)->title, (*tag2)->title ) == 0 ) {
		if( strcmp( (*tag1)->artist, (*tag2)->artist ) == 0 )
			return strcmp( (*tag1)->album, (*tag2)->album );
		return strcmp( (*tag1)->artist, (*tag2)->artist );
	}	
	return strcmp( (*tag1)->title, (*tag2)->title );						
}


/*=======================================================================================
Function Name: compareArtist

Description: 
	This function, used by qsort, compares the artists, then the albums (if the
	artists are the same) and finally the titles (if the albums are the same)

Parameters:
	const MP3Tag_t **tag1: pointer to a tag pointer
	const MP3Tag_t **tag2: pointer to another tag pointer

Retorno:
	int: (comparsion result) 	>0 (**tag1 > **tag2)
								<0 (**tag1 < **tag2)
								=0 (**tag1 == **tag2)
=======================================================================================*/ 
static int compareArtist( const MP3Tag_t **tag1, const MP3Tag_t **tag2 ) {
	if( strcmp( (*tag1)->artist, (*tag2)->artist ) == 0 ){
		if( strcmp( (*tag1)->album, (*tag2)->album ) == 0 )
			return strcmp( (*tag1)->title, (*tag2)->title );
		return strcmp( (*tag1)->album, (*tag2)->album );
	}
	return strcmp( (*tag1)->artist, (*tag2)->artist );						
}


/*=======================================================================================
Funtion Name: createBinaryTree

Description: 
	This fucntion splits the title, in a given tag, into diffenret word and
	cretes nodes of a binary to the diferent words

Parameters:
	MP3Tag_t *tag: pointer to a tag

Return:
	Doesnt have
=======================================================================================*/
static void createBinaryTree( TagSet_t *set ){
	int i;
	for ( i = 0; i < set->count; i++ ) {
		if( set->data[i] != NULL  ) {
			char temp[MAX_TIT+1];
			strcpy( temp, set->data[i]->title );							/* save the title in a temporary variable 
																				so that the  title remains unchanged */	
			int nodeInserted;	
			char *token = strtok(temp, " \t\n");
			
			while( token != NULL ){											/* go through all the words in the  title */
				char *word = strdup( token );		 							// creates a duplicate of the word in dynamic memory 
				nodeInserted = insertNode( &(set->root), word, set->data[i] );	// tries to insert the word in the binary tree
				if(nodeInserted == 0)											// if the word already exists...															 
					free(word);													// ...frees the memory alocated 
										
				token = strtok(NULL, " \t\n");
			}
		}
		if( i % 10 == 0 )
			set->root = Tbalance( set->root ); 		// balancing the tree every 10 tags read
	}
	set->root = Tbalance( set->root ); 				// final balancing of the tree
}


/**======================================================================================
Funtion Name: setReady

Description: 
	This function sorts the array pointed by data, a member of the set, by titles,
	 and the array pointed by aux, also a member of the set, by artists

Parameters:
	TagSet_t *set:	pointer to the set descriptor

Return:
	Doesnt have
=======================================================================================*/ 
void setReady( TagSet_t *set ) {				
	if( set->data != NULL ) {										// check if the array contains any element
		set->aux = malloc( sizeof(MP3Tag_t *) * set->size );		/* allocates space for the array pointed by aux, with the same 
																		dimension used by the array pointed by data */
		if( set->aux != NULL ) {									// checks if the allocation was successfull
			int i;
			for( i = 0; i < set->size; i++ )
				set->aux[i] = set->data[i];							// copies the elements in data to aux ( the pointers the tag )
			
			/* sorts the array pointed by data by title, and the array pointed by aux by artist */
			qsort( set->data, set->count, sizeof(MP3Tag_t *), (int (*)(const void *, const void *))compareTitle );
			qsort( set->aux, set->count, sizeof(MP3Tag_t *), (int (*)(const void *, const void *))compareArtist );
			
			/* creates the binary tree */
			createBinaryTree( set );
		}
	}
}


/**======================================================================================
Function Name: setCommand

Description: 
	This function, sends to the standard output, the results of the command 
	(passed to this function)

Parameters:
	TagSet_t *set:	pointer to a set descriptor
	char *cmdLine: command inserted by the user

Return:
	Doesnt have
=======================================================================================*/ 
void setCommand( TagSet_t *set, char *cmdLine ) {
	if( strcmp( cmdLine, TITLE_COMMAND ) == 0 ) {		// check if it is the sort by title command
		
		/* in case of error in some member of the tag, sends an error 
		   message to stdout related to that member only */
		int i;
		for ( i = 0; i < set->count; i++ ) {
			if( set->data[i] != NULL  ) {
				printf(" TITLE: %-31s", set->data[i]->title);
				printf(" ARTIST: %-31s", set->data[i]->artist);
				printf(" ALBUM: %-31s", set->data[i]->album);
		
				if( set->data[i]->year != YEAR_UNKNOWN )
					printf(" YEAR: %-10d", set->data[i]->year);
				else
					printf(" YEAR: %-10s", ERROR);
			
				printf(" COMMENT: %-31s", set->data[i]->comment);
		
				if( set->data[i]->track != '\0' )
					printf(" TRACK: %-10d", set->data[i]->track);
				else
					printf(" TRACK: %-10s", ERROR);
			
				if( set->data[i]->genre != GENRE_UNKNOWN )
					printf(" GENRE: %-10d", set->data[i]->genre);
				else
					printf(" GENRE: %-10s", ERROR);
			
				if( set->data[i]->fileName != NULL )
					printf(" FILE: %-10s", set->data[i]->fileName);
				else
					printf(" FILE: %-10s", ERROR);
			
				printf("\n");
			}
		}
	}
	
	if( strcmp( cmdLine, ARTIST_COMMAND ) == 0 ) {		// check if it is the sort by artist command
		
		/* in case of error in some member of the tag, sends an error 
		   message to stdout related to that member only */
		int i;
		for ( i = 0; i < set->count; i++ ) {
			if( set->aux[i] != NULL  ) {
				printf(" ARTIST: %-31s", set->aux[i]->artist);
				printf(" ALBUM: %-31s", set->aux[i]->album);
				printf(" TITLE: %-31s", set->aux[i]->title);
		
				if( set->aux[i]->year != YEAR_UNKNOWN )
					printf(" YEAR: %-10d", set->aux[i]->year);
				else
					printf(" YEAR: %-10s", ERROR);
			
				printf(" COMMENT: %-31s", set->aux[i]->comment);
		
				if( set->aux[i]->track != '\0' )
					printf(" TRACK: %-10d", set->aux[i]->track);
				else
					printf(" TRACK: %-10s", ERROR);
			
				if( set->aux[i]->genre != GENRE_UNKNOWN )
					printf(" GENRE: %-10d", set->aux[i]->genre);
				else
					printf(" GENRE: %-10s", ERROR);
			
				if( set->aux[i]->fileName != NULL )
					printf(" FILE: %-10s", set->aux[i]->fileName);
				else
					printf(" FILE: %-10s", ERROR);
			
				printf("\n");
			}
		}
	}
	
	/* if the command search was inserted the searches the word passed in the command in the binary tree
		and prints the tags of all the titles with that word */
	if( cmdLine[0] == SEARCH_COMMAND ) {
		char *word = strtok( cmdLine, " " );				// saves the word passed in the search command
		word = strtok( NULL, " " );
		int checkWord = findNode( set->root, word );		// checks if the word exists in any node
		if( checkWord == 0 ) {							
			printf(" THE WORD IS NOT MENTIONED IN ANY TITLE! ");
			printf("\n");
		}
	}	
}


/**======================================================================================
Funtion Name: setDelete

Description: 
	This function deletes the set of tags descriptor and free the allocated 
	 memory.

Parameters:
	TagSet_t *set: pointer to the set descriptor

Return:
	Doesnt have
=======================================================================================*/ 
void setDelete( TagSet_t *set ) { 
	freeTree(set->root);						// frees de mem referent to the bin tree
	
	int i;
	for ( i = 0; i < set->count; i++ ) {		// frees the mem referent every fileName
		free( set->data[i]->fileName );
	}
	for ( i = 0; i < set->count; i++ ) {		// frees the mem referent every tag
		free( set->data[i] );
	}
	free( set->data );							// free the array with pointers to tags
	free( set->aux );	
	free( set );								// frees the set descriptor
}
