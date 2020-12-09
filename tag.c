/** 
 *        name: tag.c
 *  created on: 2020-05-20
 *      author:	Daniel Barradas	            
*/ 

/* *********************  COMENTÁRIO  ********************************
 * A função tagFieldRead() foi desenvolvida, mesmo não constando no 
 * enunciado, uma vez que verificamos que existia um padrao na leitura
 * dos membros title, artist, album e comment. 
 * *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtype.h"
#include "tag.h"
#include "strset.h"

/**======================================================================================
Function Name: tagFieldRead

Description: 
	This fucntion reads a field from the tag (title, artist, album and comment)

Parameters:
	FILE *audioFile: poitnts string with the audio file name
	char field[]: fied/meber of tag to be read
	long *field_pos: position of the field in the file (in bytes)
	int size: size of the field to be read

Return:
	Doesnt have
=======================================================================================*/
static void tagFieldRead( FILE *audioFile, char field[], long *field_pos, int size ) {
	
	int error = 0;		// variable for error control
	size_t nmemb = 1;	// number of elements to be read						
	char temp = '\0';	// temporary variable to save one byte read from the file
					
	*field_pos = ftell( audioFile );	// get current file pointer 	
	
	int i;
	for( i = 0; i < size; i++) {
		if( fread( &temp, sizeof(char), nmemb, audioFile ) == nmemb ) {	// read a byte, belonging to the field, from the file
							
			if( i == 0)				// save the first byte read to the field
				field[i] = temp;			
			else{				
				// only saves the the byte being read if:
					// the byte being read is space and the last one read wasnt space
					// the byte being read isnt sapce and the last one read wasnt space
					// the byte being read is space and the last one read wasnt space
				if( ( temp == ' ' && field[i-1] != ' ' ) || ( temp != ' ' && field[i-1] != ' ' ) || ( temp != ' ' && field[i-1] == ' ' ) )
					field[i] = temp;
								
				if( temp == ' ' && field[i-1] == ' ') {	// check if the byte being read is space and the last one read was space 
					field[i-1] = '\0';					// if a space occured right after a space, then terminates the string being read
					break;
				}
			}
			if( temp == '\0')							// check if it has arrived to the end of the field being read	
				break;
		}
		else
			error = 1;
	}
	if( strcmp( SPACE, field ) == 0 )					// check if the field read is only a space
		strcpy( field, ERROR );
				
	if( field[0] == '\0' || error == 1 )				// check if the an error occured while reading the filed from the file
		strcpy( field, ERROR );
} 


/**======================================================================================
Function Name: tagRead

Description: 
	The function receives a audio file name, tries to read the tag, and if it 
	exists, stores it in a MP3Tag_t struct

Parameters:
	char *fileName: points to string with the file name
	int *resPtr: pointer to the result code of the function

Return:
	MP3Tag_t *: pointer to the tag created or NULL if the tag wasnt created
=======================================================================================*/
MP3Tag_t *tagRead( char *fileName, int *resPtr, strSet_t *sSet ) {
	
	FILE *audioFile = fopen( fileName, "rb" );					// open the file with name passed by argument
	if( audioFile != NULL ) {									// check if the file was opened successfully						

		if( fseek( audioFile, -TAG_SIZE, SEEK_END ) == 0 ) {	// seek to the first byte of the tag	
			
			char identifier[TAG_IDENTIFIER_SIZE + 1];			// string to store the first 3 bytes of the last 128 bytes of the file
			
			size_t nmemb = 1;									// number of elements to be read				
			char temp = '\0';									// temporary variable to save one byte read from the file
			
			int i;
			for ( i = 0; i < TAG_IDENTIFIER_SIZE; i++ ) {
				if( fread( &temp, sizeof(char), nmemb, audioFile ) == nmemb )	// read a byte, belonging to the tag identifier from the file
					identifier[i] = temp;
			}
			identifier[TAG_IDENTIFIER_SIZE] = '\0';				// terminate the identifier read with null char so strcmp() can be used
		
			if( strcmp( identifier, TAG_IDENTIFIER ) == 0 ){	// check if the file has tag informaton		
				
				MP3Tag_t *tag = malloc( sizeof(MP3Tag_t) );		// allocate memory for the tag
				
				if( tag == NULL ){								// check if the allocation was successfull
					
					fclose( audioFile );						// close the file if an error occured allocating memory for the tag			
					
					if( resPtr != NULL )
						*resPtr = -3;
						
					return NULL;					
				}
				
				long title_pos = 0, artist_pos = 0, album_pos = 0, year_pos = 0, comment_pos = 0;	// variables to save tag field's positions
				char tempString[MAX_STRING+1];
				
				/** ***** READ TITLE ***** **/
				tagFieldRead( audioFile, tag->title, &title_pos, MAX_TIT );		
				
				/** ***** READ ARTIST ***** **/
				if( fseek( audioFile, title_pos + MAX_TIT, SEEK_SET ) == 0 ) {		// seek to the first byte of the artist's field
					tagFieldRead( audioFile, tempString, &artist_pos, MAX_ART );
					tag->artist = strSearchAdd( sSet, tempString );				// check if the string already exists and if not, save the string
				}	
				else 
					tag->artist = strSearchAdd( sSet, ERROR );
				
				/** ***** READ ALBUM ***** **/
				if( fseek( audioFile, artist_pos + MAX_ART, SEEK_SET ) == 0 ) {		// seek to the first byte of the album's field
					tagFieldRead( audioFile, tempString, &album_pos, MAX_ALB );		
					tag->album = strSearchAdd( sSet, tempString );				// check if the string already exists and if not, save the string
				}
				else
					tag->album = strSearchAdd( sSet, ERROR );
				
				/** **** READ YEAR ***** **/
				tag->year = YEAR_UNKNOWN;			// initialize the year member from tag
				
				if( fseek( audioFile, album_pos + MAX_ALB, SEEK_SET ) == 0 ) {			// seek to the first byte of the year's field
					
					year_pos = ftell( audioFile );	// get current file pointer (year's first byte)
					
					char aux[YEAR_SIZE+1];			// auxiliary array to save the chars read from the file
					int aux2 = 0;					// auxiliary variable to help translate the char to numbers
					
					for ( i = 0; i < YEAR_SIZE; i++ ) {
						if( fread( &temp, sizeof(char), nmemb, audioFile ) == nmemb )	// read a byte, belonging to the year's field, from the file
							aux[i] = temp;
					}
					
					aux[YEAR_SIZE] = '\0';			// terminate the array of chars being read
					aux2 = atoi( aux );				// translate the charcters read to numbers
					
					if( aux2 != 0  )				// check the return value of atoi function
						tag->year = aux2;
				}
				
				/** ***** READ COMMENT ***** **/
				if( fseek( audioFile, year_pos + YEAR_SIZE, SEEK_SET ) == 0 )			// seek to the first byte of the comment's field
					tagFieldRead( audioFile, tag->comment, &comment_pos, MAX_COM );	
				else
					strcpy( tag->comment, ERROR );
				
				/** ***** READ TRACK ***** **/
				if( fseek( audioFile, comment_pos + MAX_COM-2, SEEK_SET ) == 0 ) {		// seek to the first byte of the track's field
					
					char temp2[TRACK_SIZE+1] = {' ', ' '};		// temporary variable to save the track and help check file type
					
					for (i = 0; i < TRACK_SIZE+1; i++) {
						if( fread( &temp, sizeof(char), nmemb, audioFile ) == nmemb )	// read a byte, belonging to the track's field, from the file
							temp2[i] = temp;
					}
					
					if( temp2[0] == '\0' && (temp2[1] != ' ' || temp2[1] != '\0')  )	// check the file format, ID3v1 or ID3v1.1		
						tag->track = temp2[1];											// save the byte read to the tag struct
					else
						tag->track = '\0';
				}
				else
					tag->track = '\0';
				
				/** ***** READ GENRE ***** **/
				tag->genre = GENRE_UNKNOWN;												// initialize the genre member from tag
				
				if( fseek( audioFile, comment_pos + MAX_COM, SEEK_SET ) == 0 ) {		// seek to the first byte of the genre's field
					
					if( fread( &temp, sizeof(char), nmemb, audioFile ) == nmemb ) {		// read a byte, belonging to the genre's field, from the file
						
						if( temp >= GENRE_MIN_LIM && temp <= GENRE_MAX_LIM )			// check if the genre is valid			
							tag->genre = temp;											// save the byte read to the tag struct
					}
				}
				
				/** **** SAVE FILENAME ***** **/
				char *audioFileName = malloc( sizeof(char *) * strlen(fileName) );		// allocate memory for the filename
				
				if(audioFileName != NULL)				// check if the allocation was succesfull
					strcpy( audioFileName, fileName );
				else
					strcpy( audioFileName, ERROR ); 
				
				tag->fileName = audioFileName;			// save the fileName to the tag struct
				
				fclose( audioFile );					// close the file after reading the tag
				
				if( resPtr != NULL )					// check if variable to save the function's result exists
					*resPtr = 0;
				
				return tag;
			}
			
			fclose( audioFile );						// close the file if it has no tag information		
				
			if( resPtr != NULL )						// check if variable to save the function's result exists
				*resPtr = -2;
				
			return NULL;	
		}
	}
	
	if( resPtr != NULL )	// check if variable to save the function's result exists
		*resPtr = -1;
		
	return NULL;
}
