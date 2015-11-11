/**
 This file handles all user I/O and compresses a file.
 
 @author Janne Louise Ave (jfave)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wordlist.h"
#include "bits.h"

/** Max amount commaned arguments */
#define MAX_ARGUMENTS 4

/** Min amount of command arguments */
#define MIN_ARGUMENTS 3

/** Int returned if program fails */
#define UNSUCCESSFUL_EXIT_STATUS 100

/** Amount to start of and increment by */
#define SECTION 20

/** position of the output file */
#define OUTPUT_FILE 2

/**
 Starting point of the program.
 */
int main( int argc, char *argv[] )
{
  if ( argc > MAX_ARGUMENTS || argc < MIN_ARGUMENTS ) {
    fprintf( stderr, "usage: unpack <compressed.raw> " );
    fprintf( stderr, "<output.txt> [word_file.txt]\n" );
    exit( UNSUCCESSFUL_EXIT_STATUS );
  } else {

    char *wordFile = "words.txt";
    if ( argc == MAX_ARGUMENTS )
      wordFile = argv[ MAX_ARGUMENTS - 1 ];
    WordList *wordList = readWordList( wordFile );

    FILE *input = fopen( argv[ 1 ], "rb" );
    if ( input == NULL ) {
      fprintf( stderr, "Can't open file: %s\n", argv[ 1 ] );
      freeWordList( wordList );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }

    FILE *output = fopen( argv[ OUTPUT_FILE ], "w" );
    if ( input == NULL ) {
      fprintf( stderr, "Can't open file: %s\n", argv[ OUTPUT_FILE ] );
      freeWordList( wordList );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }

    PendingBits pending = { 0, 0 };
    int index;
    while ( ( index = readCode( &pending, input ) ) != EOF )  {
      char *str = wordList->words[ index ];
      fprintf( output, "%s", str );
    }

    fclose( input );
    fclose( output );

  }

    return 0;
}
