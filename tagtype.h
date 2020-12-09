/** 
 *        name: tagtype.h
 *  created on: 2020-05-20
 *      author:	Daniel Barradas	             
*/ 
#ifndef TAGTYPE_H
#define TAGTYPE_H
 
#define MAX_TIT 30
#define MAX_ART 30
#define MAX_ALB 30
#define MAX_COM 30

typedef struct {
  char title[MAX_TIT + 1];
  char *artist;
  char *album;
  short year;
  char comment[MAX_COM + 1];
  char track;
  char genre;
  char *fileName;			// Points string with the name of the file, in dynamic memory
} MP3Tag_t;

#endif
