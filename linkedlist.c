/** 
 *        name: linkedlist.c
 *  created on: 2020-06-22
 *      author:	Daniel Barradas	        
*/ 

/* *********************  COMENTÁRIO  ********************************
 * Assume-se, nas funções deste módulo, que os elementos de cada
 * lista vão ser inseridos em ordem lexicografica, uma vez que a
 * ordeção das tags é feita antes da criação da árvore binária.
 * (onde vão ser inseridas as listas)
 * *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtype.h"
#include "tag.h"
#include "linkedlist.h"

/**======================================================================================
Function Name: findElem

Description:
	This function searches for the last element on the list

Parameters:
	TreeListNode *head: reference to the head of the list

Return:
	TreeListNode *: reference to last element of the list
=======================================================================================*/
TreeListNode *findElem( TreeListNode *head ) {
	TreeListNode *prev = head;
	
	while(prev->next != NULL)			// checks if the current elem has a next elem
		prev = prev->next;				// saves the next elem to check in the next iteration 

	return prev;						// returns the tail of the linked list
}


/**======================================================================================
Function Name: insertElem

Description: 
	This function inserts an element to a linked list

Parameters:
	TreeListNode *head: pointer to the head (first element of the list) 
	MP3Tag_t *tag: pointer to a tag in dynamic memory

Return:
	Doesnt have
=======================================================================================*/
void insertElem( TreeListNode *head, MP3Tag_t *tag ) {
	TreeListNode *tail, *new;
	
	tail = findElem(head);
	if( tail->tag != tag ) {
		new = malloc(sizeof(TreeListNode));
		if(new == NULL) {
			fprintf(stderr, "%s (line %d): COULD NOT ALLOCATE MEMORY.\n",  __FILE__,  __LINE__);
			exit(1);
		}
		
		/* Inicialization of the pointers of the new element */
		new->next = NULL;	
		new->tag = tag;
		
		/* Insertion of the new element in the list */
		tail->next = new;
	}
}


/**======================================================================================
Function Name: printList
 
Description:
	This function prints a list of elements with at least one word equal

Parameters:
	TreeListNode *head: pointer to the head (first element of the list) 

Return:
	Doesnt have
=======================================================================================*/
void printList( TreeListNode *head ) {
	if(head->tag != NULL){
		printf(" TITLE: %-31s", head->tag->title);
		printf(" ARTIST: %-31s", head->tag->artist);
		printf(" ALBUM: %-31s",head->tag->album);
				
		if( head->tag->year != YEAR_UNKNOWN )
			printf(" YEAR: %-10d", head->tag->year);
		else
			printf(" YEAR: %-10s", ERROR);
					
		printf(" COMMENT: %-31s", head->tag->comment);
				
		if( head->tag->track != '\0' )
			printf(" TRACK: %-10d", head->tag->track);
		else
			printf(" TRACK: %-10s", ERROR);
					
		if( head->tag->genre != GENRE_UNKNOWN )
			printf(" GENRE: %-10d", head->tag->genre);
		else
			printf(" GENRE: %-10s", ERROR);
					
		if( head->tag->fileName != NULL )
			printf(" FILE: %-10s", head->tag->fileName);
		else
			printf(" FILE: %-10s", ERROR);
					
		printf("\n");
	}
			
	if(head->next != NULL)
		printList(head->next);
}


/**======================================================================================
Function Name: freeList

Description:
	This fucntion frees a linked list of a tree node

Parameters:
	TreeListNode *head: pointer to the head (first element of the list) 

Return:
	Doesnt have
=======================================================================================*/
void freeList( TreeListNode *head ) {
	if(head->next != NULL)			// checks if there are more elements and if positive frees them
		freeList( head->next );		
	
	free(head);						// frees the element passed to the fucntion
}
