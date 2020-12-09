/** 
 *        name: binarytree.c
 *  created on: 2020-06-22
 *      author:	Daniel Barradas             
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtype.h"
#include "linkedlist.h"
#include "binarytree.h"

/**======================================================================================
Function Name: insertNode

Description: 
	This fucntions inserts a node to the tree. It seaches the tree for the node with
	the same string 'word'. If the node with the word exists
	then it inserts a new element to the linked list. If the node doesnt exist
	it creates the node and creates and inicializes the head to the linked list of that
	node

Parameters:
	TNode **root_p: reference to a pointer to the root of the tree
	char *word: string with a word beloging to one or various titles
	MP3Tag_t *tag: pointer to a tag in dynamic memory

Return:
	int: 1 if it created the node or 0 if the already existed
=======================================================================================*/
int insertNode( TNode **root_p, char *word, MP3Tag_t *tag ) {
	if(*root_p == NULL) {
		*root_p = malloc(sizeof(TNode));
		if(*root_p == NULL) {
			fprintf(stderr, "%s (line %d): COULD NOT ALLOCATE MEMORY.\n",  __FILE__,  __LINE__);
			exit(1);
		}
		
		/* Initilization of the members of a node */
		(*root_p)->word = word;							
		(*root_p)->list = malloc(sizeof(TreeListNode));		// allocates memory for the head of a linked list
		if((*root_p)->list == NULL) {
			fprintf(stderr, "%s (line %d): COULD NOT ALLOCATE MEMORY.\n",  __FILE__,  __LINE__);
			exit(1);
		}
		
		(*root_p)->list->next = NULL;
		(*root_p)->list->tag = tag;
		(*root_p)->left = (*root_p)->right = NULL; 		
			
		return 1;											// stop the fucntion, pointing out that the node was created 
	}
	int aux = strcmp(word, (*root_p)->word);				/* Compare the word passed by parameter with the word in the node */
	if(aux == 0) {												// if the words are equal
		insertElem((*root_p)->list, tag);						// inserts an element to the linked list of that node	
		return 0;												// stop the function, pointing out that the node wasnt created
	}
	if(aux < 0)	{												// if the word is lower than the word in the node (lexicographically)
		return insertNode(&((*root_p)->left), word, tag);		// try to create a new node to the left
	}															// if the word is bigger than the word in the node (lexicographically)
	return insertNode(&((*root_p)->right), word, tag);			// try to create a new node to the right
}


/**======================================================================================
Function Name: findNode

Description: 
	This fucntion searches for a word in the binary tree. The search can start
	at any node passed by parameter

Parameters:
	TNode *root: first node where the seach begins
	char *word: word to be found

Return:
	int: 1 if it found the node or 0 if it didnt exists
=======================================================================================*/
int findNode( TNode *root, char *word ) {
	if(root == NULL)
		return 0;
	
	int aux = strcmp(word, root->word);					/* Compare the word passed by parameter with the word in the node */
	if(aux == 0) {										// if the words are equal
		printList(root->list);
		return 1;										// stop the function, pointing out that the word was found
	}
	if(aux < 0)											// if the word is lower than the word in the node (lexicographically)
		return findNode(root->left, word);				// try to find a new node to the left
														// if the word is bigger than the word in the node (lexicographically)
	return findNode(root->right, word);					// try to find a new node to the right
}

	
/*=======================================================================================
Function Name: countNodes

Description: 
	This fucntion counts the number of node in a binary tree

Parameters:
	TNode *root: pointer to first element of the tree (root)

Retorno:
	long: number of nodes
=======================================================================================*/
static long countNodes( TNode *root ) {
	int count = 1;
	if(root == NULL)	
		return 0;
	
	count += countNodes(root->left);			// counts all the nodes to the left
	count += countNodes(root->right);			// counts all the nodes to the right
	
	return count;
}


/*=======================================================================================
Funtion Name: treeToSortedList

Description: 
	This function degenates a tree to a linked list to the right

Parameters:
	TNode *r: first node of the tree (root)
	TNode *link: node that precedes the node pointed by r. When r is the root, 
				 the link parameter should be NULL

Return:
	Tnode *: new root of the tree (head of the linked list)
=======================================================================================*/
static TNode *treeToSortedList( TNode *r, TNode *link ) {
	TNode * p;
	if( r == NULL ) 
		return link;
		
	p = treeToSortedList( r->left, r );
	r->left = NULL;
	r->right = treeToSortedList( r->right, link );
	
	return p;
} 


/*======================================================================================
Function Name: sortedListToBalancedTree

Description: 
	This fucntion balances a binary tree that is degenareted into a linked list
	to the right

Parameters:
	TNode **list: fisrt element of the list
	int n: number of nodes of the list (degenerated tree)

Return:
	Tnode *: new root of the tree
=======================================================================================*/
static TNode *sortedListToBalancedTree( TNode **listRoot, int n ) {
    if( n == 0 ) 
		return NULL;
		
    TNode *leftChild = sortedListToBalancedTree(listRoot, n/2);
    TNode *parent = *listRoot;
    parent->left = leftChild;
    *listRoot = (*listRoot)->right;
    parent->right = sortedListToBalancedTree(listRoot, n-(n/2 + 1) );
    
    return parent;
}


/**======================================================================================
Function Name: Tbalance

Description: 
	This fucntion balances a binary tree.

Parameters:
	TNode *root: first node of the tree (root)

Return:
	Tnode *: new root of the tree
=======================================================================================*/
TNode *Tbalance( TNode *root ){
	
	/* degenerates the initial binary tree into a linked list */
	root = treeToSortedList( root, NULL );				
	/* creates the final binary tree, balancing the degenerated tree */
	root = sortedListToBalancedTree( &(root), countNodes(root) );
	
	return root;
}


/**======================================================================================
Funtion Name: freeNode

Description: 
	This fucntion frees memory alocated by a node of the tree

Parameters: 
	TNode *node: node of the tree to wich its memory will be freed

Return:
	Doesnt have
=======================================================================================*/
static void freeNode( TNode *node ) {
	freeList(node->list);					// free the mem referent to the linked list
	free(node->word);						// free the mem referent to the word
	free(node);								// free the mem referent to the node
}


/**======================================================================================
Function Name: freeTree

Description: 
	This fucntion free the memory alocated by all nodes of the tree

Parameters: 
	TNode *node: pointer to the root of the tree

Return:
	Doesnt have
=======================================================================================*/
void freeTree( TNode *root ) {
	if(root == NULL)
		return;
	freeTree(root->right);
	freeTree(root->left);
	freeNode(root);
}
