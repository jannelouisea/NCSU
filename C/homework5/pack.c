/**
 This file manages reading in the file and
 calling the appropriate functions.
 
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

/** Mask full of ones */
#define FULL_MASK 0xFF

/**
 This function reads in the input file and returns a giant
 string containing the contents of the file.
 
 @param fp pointer to the FILE.
 @return a pointer to the giant string.
 */
char *readFile( FILE *fp )
{
  int capacity = SECTION;
  char *buffer = malloc( capacity );
  int len = 0;
  char c = ' ';
  while ( fscanf( fp, "%c", &c ) == 1 ) {
    if ( validChar(c) == 0 ) {
      fprintf( stderr, "Invalid character code: %02x\n", c & FULL_MASK );
      free( buffer );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }
    if ( len >= capacity ) {
      buffer = realloc( buffer, capacity += SECTION );
    }
      buffer[len] = c;
      len++;
  }
  buffer[len] = '\0';
  return buffer;
}

/**
 Main starting point about the program.
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

    FILE *input = fopen( argv[ 1 ], "r" );
    if ( input == NULL ) {
      fprintf( stderr, "Can't open file: %s\n", argv[ 1 ] );
      fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [word_file.txt]\n" );
      freeWordList( wordList );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }

    char *buffer = readFile( input );
    fclose( input );

    FILE *output = fopen( argv[ OUTPUT_FILE ], "w" );
    if ( output == NULL ) {
      fprintf( stderr, "Can't open file: %s\n", argv[ OUTPUT_FILE ] );
      fprintf( stderr, "usage: unpack <compressed.raw> " );
      fprintf( stderr, "<output.txt> [word_file.txt]\n" );
      freeWordList( wordList );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }

    int position = 0;
    PendingBits pending = { 0, 0 };
    while ( buffer[ position ] ) {
      int code = bestCode( wordList, buffer + position );
      writeCode( code, &pending, output );
      position += strlen( wordList->words[ code ] );
    }

    if ( pending.bitCount != 0 )
      flushBits( &pending, output );

    fclose( output );
    freeWordList( wordList );
    free( buffer );
  }
    return 0;
}
