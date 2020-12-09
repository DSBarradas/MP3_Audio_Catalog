/** 
 *        name: tag.h
 *  created on: 2020-05-20
 *      author:	Daniel Barradas	      
*/ 
#ifndef TAG_H
#define TAG_H

#include "tagtype.h"
#include "strset.h"
 
#define TAG_IDENTIFIER "TAG"
#define TAG_IDENTIFIER_SIZE 3
#define TAG_SIZE 128

#define YEAR_SIZE 4
#define YEAR_UNKNOWN 0

#define TRACK_SIZE 1

#define GENRE_SIZE 1
#define GENRE_UNKNOWN -1
#define GENRE_MAX_LIM 125
#define GENRE_MIN_LIM 0

#define SPACE " "
#define ERROR "|unknown|"

#define MAX_STRING 30

MP3Tag_t *tagRead( char *fileName, int *resPtr, strSet_t *sSet );

#endif
