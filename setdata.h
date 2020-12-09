/** 
 *        name: setdata.h
 *  created on: 2020-05-24
 *      author:	Daniel Barradas	        
*/ 
#ifndef SETDATA_H
#define SETDATA_H

#include "tagtype.h"
#include "binarytree.h"

#define COMMAND_SIZE 1
#define TITLE_COMMAND "t"
#define ARTIST_COMMAND "a"
#define SEARCH_COMMAND 's'

typedef struct{
  int size;				// quantity of elements allocated in data
  int count;			// quantity of elements stored in data
  MP3Tag_t **data;		// points an array in dynamic memory with pointers to the tags
  MP3Tag_t **aux;		// aux points to the same elements of data but with different order
  TNode *root;			// root of the binary tree	
} TagSet_t;					

TagSet_t *setCreate( void );

void setAdd( TagSet_t *set, MP3Tag_t *tag );

void setReady( TagSet_t *set );

void setCommand( TagSet_t *set, char *cmdLine );

void setDelete( TagSet_t *set );

#endif
