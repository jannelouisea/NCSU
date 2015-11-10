/**
    @file wordlist.c
    @author Ian Baldwin (itbaldwi)

    Functions used to work with WordList objects
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "wordlist.h"

/** The integer value for a horiziontal tab character */
#define HTAB 0x09

/** The integer value for a new line character */
#define NEWLINE 0x0A

/** The integer value for a carriage return character */
#define CRETURN 0x0D

/** The integer value for a space character */
#define SPACE 0x20

/** The integer value for a tilde character */
#define TILDE 0x7E

/** The number of valid characters and thus the minimum length of a dicitionary */
#define MINLENGTH 98

/** The maximum amount of words that a WordList may add beyond the default */
#define MAXADDITIONAL 414


/**
    Prints an error message and exits when an invalid
    situation is encountered
 */
void exitInvalid() {
    fprintf( stderr, "Invalid word file\n" );
    exit( EXIT_FAILURE );
}

bool validChar( char ch )
{
    if ( ch == HTAB )
        return true;
    else if ( ch == NEWLINE )
        return true;
    else if ( ch == CRETURN )
        return true;
    else if ( ch >= SPACE && ch <= TILDE )
        return true;
    else 
        return false;
}

/**
    A function used to compare the contents of an array of Words for a WordList

    @param *s1 A pointer to the first String to compare
    @param *s2 A pointer to the second String to compare
    @return The differnce between the two Strings
 */
int compareFunc( void const *s1, void const *s2 )
{
    return strcmp( ( char * ) s1, ( char * ) s2 );
}

WordList *readWordList( char const *fname )
{
    // Creates a new WordList
    WordList *list = ( WordList * )malloc( sizeof( WordList ) );
    
    // Sets the initial length of the WordList to 0
    list->len = 0;

    // Sets the initial capacity of the WordList to it's minimum value
    list->capacity = MINLENGTH;

    // Dynamically allocates the list of words to the minimum size
    list->words = ( Word * )malloc( list->capacity * sizeof( Word )  );

    // Creates a pointer to the file that contains additional words
    FILE *file = fopen( fname, "r" );

    // Checks that the file exists
    if ( !file ) {
        fprintf( stderr, "Can't open word file: %s\n", fname );
        exit( EXIT_FAILURE );
    }

    // The count of characters in a given Word in the WordList
    int charCount = -1;

    // An arbitrary word
    Word word;

    // An arbitrary character
    char ch;

    // Read in the words of a word list
    while ( fscanf( file, "%d%*c", &charCount ) != EOF ) { // Get the digit and skip the space
        
        // Check to see if fscanf was able to read an integer
        if ( charCount == -1 )
            exitInvalid();

        // Check to see if the length of the word is allowed
        if ( charCount < WORD_MIN || charCount > WORD_MAX )
            exitInvalid();

        // Check if the length of the Wordlist is valid
        if ( list->len == MAXADDITIONAL )
            exitInvalid();

        // Read in the amount of characters specified in the wordlist file 
        for ( int i = 0; i < charCount; i++ ) {
            fscanf( file, "%c", &ch );
            word[ i ] = ch; 
        }

        // Null-terminate the word
        word[ charCount ] = '\0'; 

        // Enlargement check
        if ( list->capacity == list->len ) { 
            list->capacity *= 2; 
            list->words=( Word * )realloc( list->words, list->capacity * sizeof( Word ) ); 
        }


        // Checks to see if all of the characters in the Word are valid
        for ( int i = 0; i < charCount; i++ ) { 
            if ( !validChar( word[ i ] ) )
                exitInvalid();
        }

        // Checks to see if the Word has already been added to the WordList 
        for ( int i = 0; i < list->len; i++ ) { 
            if ( !strcmp( list->words[ i ], word ) )
                exitInvalid();
        }
        
        // Copy the Word into the WordList
        strcpy( list->words[ list->len++ ], word );

        // Reset the character count
        charCount = -1;
    }

    // Close the pointer to the input file
    fclose( file );

    // Put the 98 valid characters in the WordList
    for ( int i = HTAB; i <= TILDE; i++ ){
        if ( validChar( i ) ) {
            char str[ 2 ];
            sprintf( str, "%c", i );
            strcpy( list->words[ list->len++ ], str );
        }
    }
    
    // Sort the WordList
    qsort( list->words, list->len, sizeof( list->words[ 0 ] ), compareFunc ); 
    
    // Return the WordList
    return list;
}

int bestCode( WordList *wordList, char const *str )
{
    // A pointer to the biggest prefix of a given String contained in a WordList that
    Word *ptr;
    
    // Starts with the whole String and works its way done
    for ( int i = 0; i < strlen( str ); i++ ) {
        Word prefix;
        strncpy( prefix, str, strlen( str ) - i );
        prefix[ strlen( str ) - i ] = '\0';
        ptr = bsearch( prefix, wordList->words, wordList->len, sizeof( wordList->words[ 0 ] ), compareFunc );   
        if ( ptr != NULL )
            break;
    } 

    // Give the index of the prefix in the WordList
    return ptr - wordList->words;
}

void freeWordList( WordList *wordList ) 
{
    free( wordList-> words );
    free( wordList ); 
}
