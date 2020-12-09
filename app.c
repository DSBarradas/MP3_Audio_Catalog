/** 
 *        name: app.c
 *  created on: 2020-05-24
 *      author:	Daniel Barradas	             
*/ 

/* *********************  COMENTÁRIO  ********************************
 * No ficheiro README.txt poderá encontrar uma lista de versões,
 * onde a TERCEIRA versão é referente à terceira série de exercicios.
 * 
 * O programa funciona como pretendido, tendo sido testado para 
 * diversos casos de erro pelo que até ao momento não foi encontrada
 * nenhuma falha.
 * *******************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtype.h"
#include "strset.h"
#include "setdata.h"
#include "listfiles.h"

#define MAX_INPUT_SIZE COMMAND_SIZE+1+MAX_TIT+1
#define QUIT_COMMAND "q"

/*=======================================================================================
Funtion Name: inputError

Description: 
	This function sends a message to standard error if the input is bigger
	then the max input defined. After sending the message, flushes the 
	input buffer

Parameters:
	char *input: buffer com os caracteres introduzidos pelo utilizador

Return:
	Doesnt have
=======================================================================================*/
static void inputError( char *inputBuffer ) {
	if( inputBuffer[0] == 's' && inputBuffer[1] == ' ' )
		fprintf(stderr, "\nINSERTED SEARCH COMMAND BUT INVALID WORD (WORD INSERTED TOO LONG)! PLEASE INSERT AGAIN.\n");
	else
		fprintf(stderr, "\nCOMMAND NOT FOUND! PLEASE INSERT AGAIN.\n");
	
	/* clear input buffer */
	char c = '\0';
	while((c = getchar()) != '\n' && c != EOF ){
		// do nothing until input buffer fully flushed
	}
}


/**======================================================================================
Function Name: main
=======================================================================================*/ 
int main( int argc, char *argv[] ) {
	
	char *prog = argv[0];		// program name for errors
	
	// check if the arguments passed to the app are valid
	if( argc == 1 ) {
		fprintf(stderr, "%s: NO ARGUMENTS \n", prog);
		exit(1);
	}
	if( argc != 1  && argc > 2) {
		fprintf(stderr, "%s: TOO MANY ARGUMENTS \n", prog);
		exit(1);
	}
	
	TagSet_t *set = setCreate();		// create a descriptor for the set of tags to be read
	strSet_t *sSet = strSetCreate();	// create a descriptor for the set of strings (with artists and albuns)
	
	if( set != NULL && sSet != NULL ) {		// check if the set was successfully created
		
		int checkList = listScan( argv[1], set, sSet );
		
		if( checkList == 0 ) {
		
			if(set->data != NULL ) {	
				setReady( set );
				
				if( set->aux != NULL ) {
					
					char *inputBuffer = malloc( sizeof(char) * (MAX_INPUT_SIZE+1) ); // variable to save the command inserted by the user
					memset(inputBuffer, '\0', MAX_INPUT_SIZE+1);				   // initialize the buffer
						
					int quit = 0; 												 /* variable to check if the quit command was inserted by the user */
					
					printf("******* AUDIO CATALOG *******");
					
					while( quit != 1 ) {
					
						printf("\nCOMMANDS LIST:\n\t-->SORT BY TITLE - <t>\n\t-->SORT BY ARTIST - <a>\n\t-->SEARCH WORD (in title) - <s 'word'>\n\t-->QUIT - <q>\nINSERT COMMAND: ");
						if( fgets( inputBuffer, MAX_INPUT_SIZE+1, stdin ) != NULL ) {
							if( inputBuffer[strlen(inputBuffer)-1] != '\n' )
								inputError( inputBuffer );
							else if( inputBuffer[0] != '\n' ) {
								char *cmdLine = strtok(inputBuffer, "\n");	// remove the '\n' char stored by fgets
							
								/* check the command inserted by the user */
								if( strcmp( cmdLine, ARTIST_COMMAND) == 0 || strcmp( cmdLine, TITLE_COMMAND) == 0 ) {
									printf("\n");
									setCommand( set, cmdLine );
								}
								else if( cmdLine[0] == SEARCH_COMMAND && (cmdLine[1] == ' ' || cmdLine[1] == '\0')) {			// check if it was inserted a valid search command
									if( cmdLine[1] != '\0' && cmdLine[1] == ' ' && cmdLine[2] != ' ' && cmdLine[2] != '\0' ){
										printf("\n");
										setCommand( set, cmdLine );
									}
									else
										fprintf(stderr,"\nINSERTED SEARCH COMMAND BUT NO WORD OR INVALID WORD! PLEASE INSERT AGAIN.\n");
								}
								else if( strcmp( cmdLine, QUIT_COMMAND) == 0 ) {
									printf("\n<EXITED PROGRAM>\n");
									setDelete( set );
									strSetDelete( sSet );
									quit = 1;
								}
								else
									fprintf(stderr,"\nCOMMAND NOT FOUND! PLEASE INSERT AGAIN.\n");
							}
							else
								fprintf(stderr,"\nNO COMMAND INSERTED! PLEASE INSERT AGAIN.\n");
						}
						else
							fprintf(stderr,"\nERROR OCCURED WHILE READING COMAND! PLEASE INSERT AGAIN.\n");
					}
					free( inputBuffer );
					
					return 0;
				}
			}
		}
		else
			if( checkList == -1 )
				fprintf(stderr, "%s: ERROR READING LIST\n", prog);
	}
	return 0;
}
