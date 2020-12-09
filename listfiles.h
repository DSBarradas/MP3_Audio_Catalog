/** 
 *        name: listfiles.h
 *  created on: 2020-05-24
 *      author:	Daniel Barradas	           
*/ 
#ifndef LISTFILES_H
#define LISTFILES_H

#include "strset.h"
#include "setdata.h"

#define MAX_FILE_NAME 255  	// Linux has a maximum filename lenght of 255 for most filesystems

int listScan( char *listName, TagSet_t *set, strSet_t *sSet );

#endif
