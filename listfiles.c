/** 
 *        name: listfiles.c
 *  created on: 2020-05-24
 *      author:	Daniel Barradas	            
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tag.h"
#include "strset.h"
#include "setdata.h"
#include "listfiles.h"

/**======================================================================================
Function Name: listScan

Description:
	This function analyses the file, with name passed by argument, that has the
	name of the audio files stored in each line. 
	For each file, uses the funtion tagRead() to know if the file has tag info,
	and if the file has tag info, adds it to the set of tags using setAdd(). 

Parameters:
	char *listName: pointer to string with the name of the file that has the list of
					names of the audio files to read
	TagSet_t *set: pointer to a set of tags descriptor

Return:
	int: 0 if it was succssefull
		-1 if that is no list with the files names
		-2 if it couldnt open/read any of the files
=======================================================================================*/ 
int listScan( char *listName, TagSet_t *set, strSet_t *sSet ) {
	
	char temp[MAX_FILE_NAME+1]; 	// temporary variable to store the name of the file read by fgets()
	int res = 0;					// variable to check the result of the function tagRead()
	
	FILE *fileList = fopen( listName, "r" );					// open the file
	if( fileList != NULL ) {									// check if the file was opened
		
		int filesOpened = 0, tagsRead = 0, tagsAllocated = 0;	// variables to keep track of the elements analysed
		
		while( fgets( temp, sizeof(temp), fileList ) != NULL ) {	
			
			char *fileName = strtok(temp, "\n");				// remove the '\n' char stored by fgets in the fileName
			
			if( fileName != NULL )
				setAdd( set, tagRead( fileName, &res, sSet ) );		// add tag to the set of tags, if it exists
			
			/* check the result of tagRead, to evaluete possible errors */
			if( res != -1 )
				filesOpened++;
			if( res != -2 )
				tagsRead++;
			if( res != -3 )
				tagsAllocated++;
						
			if( res == -1 )
				fprintf(stderr, " ERROR: COULD NOT ACCESS FILE <%s>\n\n", fileName);
			if( res == -2 )
				fprintf(stderr, " ERROR: FILE <%s> DOESNT HAVE A TAG\n\n", fileName);
			if( res == -3 )
				fprintf(stderr, " ERROR: COULD NOT ALLOCATE MEMORY FOR TAG IN FILE <%s>\n\n", fileName);
		}
		
		/* check if no file was opened, no tags were read or no memory could be allocated */
		if( filesOpened == 0 || tagsRead == 0 || tagsAllocated == 0 ) {
			fprintf(stderr, "[WARNING! ERROR OPENING ALL FILES OR NO FILE HAS TAG INFO]\n(ERROR ALLOCATING MEMORY FOR TAG INFO MIGHT HAVE OCCURED FOR ALL FILES)\n\n");
			
			return -2;
		}
		return 0;
	}
	return -1;
}
