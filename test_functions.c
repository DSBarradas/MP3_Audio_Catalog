/** 
 *        name: test_functions.h
 *  created on: 2020-06-25
 *      author:	Daniel Barradas	        
*/


/**************************************** BINARY TREE *****************************************/
/*=======================================================================================
Funtion Name: printNode

Description: 
	This fucntion prints a node of a binary tree

Parameters: 
	char *: word of the node
	int h: height of the node

Return:
	Doesnt have
=======================================================================================*/
static void printNode(char *word, int h) {
  printf("(%d)", h);
  while(h--)
    printf("  ");
  printf("%s\n", word); 
}


/*=======================================================================================
Funtion Name: printTree

Description: 
	This fucntion prints a binary tree

Parameters: 
	TNode *root: root of the binary tree (first elem)
	int h: height of the node

Return:
	Doesnt have
=======================================================================================*/
static void printTree(TNode *root, int h) {
	if(root == NULL)
		return;
	printTree(root->right, h+1);
	printNode(root->word, h);
	printTree(root->left, h+1);
}
/**********************************************************************************************/

/***************************************** HASHTABLE ******************************************/
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
	for( i = 0; i < sSet->hashsize; i++ )
		if( sSet->hashtable[i] != NULL ) {
			printf("i = %d\n", i);
		for( entry = sSet->hashtable[i]; entry != NULL; entry = entry->next )	// go through the linked list
			printf("\tname = %s\n", entry->string);
			printf("\n");
		}
				
	printf("\nNUMBER OF ENTRIES OF THE HASTABLE: %d\n\n", sSet->entries);
}
/**********************************************************************************************/

