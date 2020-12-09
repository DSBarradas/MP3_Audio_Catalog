/** 
 *        name: linkedlist.h
 *  created on: 2020-06-22
 *      author:	Daniel Barradas	          
*/ 
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "tagtype.h"

typedef struct treeListNode {
	struct treeListNode *next;	// pointer to the next element of the list
	MP3Tag_t *tag;				// pointer to a tag in dynamic memory
} TreeListNode;					/* linked list type that can exist in each node of the binary tree */ 

TreeListNode *findElem( TreeListNode *head );

void insertElem( TreeListNode *head, MP3Tag_t *tag );

void printList( TreeListNode *head );

void freeList( TreeListNode *head );

#endif
