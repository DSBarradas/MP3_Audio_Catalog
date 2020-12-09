/** 
 *        name: binarytree.h
 *  created on: 2020-06-22
 *      author:	Daniel Barradas          
*/ 
#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "tagtype.h"
#include "linkedlist.h"

typedef struct tNode {
  struct tNode *left;
  struct tNode *right;
  char *word;
  TreeListNode *list;
} TNode;

int insertNode( TNode **root_p, char *word, MP3Tag_t *tag );

int findNode( TNode *root, char *word );

TNode *Tbalance( TNode *root );

void freeTree( TNode *root );

#endif
