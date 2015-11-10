/**
  @file wordlist.c
  @author Erin Snider (eesnider)
  A file that handles the list of words that can be compressed/decompressed
  */
  
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "wordlist.h"

/** Checks the validity of the character
    @param ch the current character
    @return true if the character is valid
*/
bool validChar( char ch )
{
  if ( ch == 0x09 ) {
    return true;
  } else if ( ch == 0x0A ) {
    return true;
  } else if ( ch == 0x0D) {
    return true;
  } else if ( (ch >= 0x20 ) && ( ch <= 0x7E ) ) {
    return true;
  } else {
    return false;
  }
}

void exitInvalid() {
    fprintf( stderr, "Invalid word file\n" );
    exit( -1 );
}

int compareFunc( void const *s1, void const *s2 )
{
    return strcmp( ( char * ) s1, ( char * ) s2 );
}

/** Builds the word list and sorts it
    @param fname name of the file to build the wordList from
    @returns a struct of WordList
*/
WordList *readWordList( char const *fname )
{
    // Malloc the WordList
    WordList *list = ( WordList * )malloc( sizeof( WordList ) );
    list->len = 0;
    list->capacity = 98;
    list->words = ( Word * )malloc( list->capacity * sizeof( Word )  );
    FILE *file = fopen( fname, "r" );

    int charCount = -1;
    char word[ 21 ];
    char ch;
    // Read in the rest of WordList
    while ( fscanf( file, "%d%*c", &charCount ) != -1 ) {
        
        for ( int i = 0; i < charCount; i++ ) {
            fscanf( file, "%c", &ch );
            word[ i ] = ch;
        }
        word[ charCount ] = '\0';
        if ( list->capacity == list->len ) { // Double capacity
            list->capacity *= 2;
            list->words=( Word * )realloc( list->words, list->capacity * sizeof( Word ) );
        }

         /********************
         * ERROR CHECK BLOCK *
         ********************/

        if ( list->len == 414 ) { // Check if the length of the wordlist is invalid
            printf( "List filled\n" );
            exitInvalid();
        }

        if ( charCount == -1 ) { // See if it reads an integer
            printf( "Bad string length for for string: %s on line: %d\n", word, list->len );
            exitInvalid();
        } else if ( charCount != strlen( word ) ) { // See if it charCount and length of the string are the same
            printf( "-> The given string length (%d)  doesn't match the actual string length(%lu) for word: %s on line: %d\n", charCount, strlen( word ),  word, list->len );
            exitInvalid();
        } else if ( charCount < 2 || charCount > 20 ) { // See if the charCount is valid
            printf( "-> The given string length is invalid for word: %s on line: %d\n", word, list->len );
            exitInvalid();
        }

        for ( int i = 0; i < charCount; i++ ) { // Checks to see if all of the characters are valid
            if ( !validChar( word[ i ] ) ) {
                printf( "-> The given string contains a valid character: %d on line: %d\n", word[ i ], list->len );
                exitInvalid();
            }
        }

        for ( int i = 0; i < list->len; i++ ) { // Checks to see if the word is a duplicate
            if ( !strcmp( list->words[ i ], word ) ) {
                printf( "-> The given string is a duplicate of: %s on line: %d\n", word, list->len );
                exitInvalid();
            }
        }

        /*************************
         * END ERROR CHECK BLOCK *
         *************************/
        strcpy( list->words[ list->len++ ], word );
    }

    // Put the 98 Valid characters in the WordList
    for ( int i = 0x00; i < 0x7F; i++ ){
        if ( validChar( i ) ) {
            char str[ 2 ];
            sprintf( str, "%c", i );
            strcpy( list->words[ list->len++ ], str );
        }
    }
    
    qsort( list->words, list->len, sizeof( list->words[ 0 ] ), compareFunc ); 
    return list;
}

/** Finds best fitting corresponding word 
    @param wordList the list of available words
    @param str current string to check for
    @return the corresponding hex code for the best word
*/
int bestCode( WordList *wordList, char const *str )
{
    Word *ptr;
    for ( int i = 0; i < strlen( str ); i++ ) {
        char prefix[ 21 ];
        strncpy( prefix, str, strlen( str ) - i );
        prefix[ strlen( str ) - i ] = '\0';
        ptr = bsearch( prefix, wordList->words, wordList->len, sizeof( wordList->words[ 0 ] ), compareFunc );   
        if ( ptr != NULL )
            break;
    } 
    return ptr - wordList->words;
}

/** Frees the dynamically allocated memory for wordList
    @param wordList the list of appropriate words
*/
void freeWordList( WordList *wordList )
{
  free( wordList->words );
  free( wordList );
}
