/**
  @file pack.c
  @author Erin Snider (eesnider)
  Reads from a file and compresses it to corresponding hex code
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wordlist.h"
#include "bits.h"

/** Reads each character of the file into a buffer if they are all valid
    @param fp the input file
    @return the buffer of the entire file
 */
char *readFile( FILE *fp )
{ 
  int capacity = 5;
  char *buffer = malloc( capacity );

  int len = 0;
  
  char a;
  while( !feof( fp ) ) {
    fscanf( fp, "%c", &a );
    if ( validChar( a ) ) {
      if ( capacity == ( len + 1 ) ) {
        capacity += capacity;
        buffer = ( char* )realloc( buffer, capacity );
        buffer[ capacity ] = '\0';
      }
      buffer[ len ] = a;
      len++;
    } else {
      fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [wordfile.txt]\n" );
      exit( 1 );
    }
  }
  return buffer;
}

/** Opens appropriate files and handles them
    @param argc number of command line arguments 
    @param argv array of command line arguments
    @return the exit status of the program
  */
int main( int argc, char *argv[] )
{
  char *wordFile = "words.txt";
  
  FILE *input;
  FILE *output;
  
  if ( argc < 3 || argc > 4 ) {
    fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [wordfile.txt]\n" );
    exit( 1 );
  }
  input = fopen( argv[ 1 ], "r" );
  if ( !input ) {
    printf( "usage: readFile <input-file>\n" );
    exit( 1 );
  }
  output = fopen( argv[ 2 ], "wb" );
  if ( !output ) {
    printf( "usage: readFile <input-file>\n" );
    exit( 1 );
  }
  if ( argc == 4 ) {
    wordFile = argv[ 3 ];
  }
  
  WordList *wordList = readWordList( wordFile );

#ifdef DEBUG
  // Report the entire contents of the word list, once it's built.
  printf( "---- word list -----\n" );
  for ( int i = 0; i < wordList->len; i++ )
    printf( "%d == %s\n", i, wordList->words[ i ] );
  printf( "--------------------\n" );
#endif

  // ... 

  // Read the contents of the whole file into one big buffer.  This could be more
  // efficient, but it simplifies the rest of the program.
  char *buffer = readFile( input );

  // Write out codes for everything in the buffer.
  int pos = 0;
  PendingBits pending = { 0, 0 };
  while ( buffer[ pos ] ) {
    // Get the next code.
    int code = bestCode( wordList, buffer + pos );
#ifdef DEBUG
    printf( "%d <- %s\n", code, wordList->words[ code ] );
#endif

    // Write it out and move ahead by the number of characters we just encoded.
    writeCode( code, &pending, output );
    pos += strlen( wordList->words[ code ] );
  }

  // Write out any remaining bits in the last, partial byte.
  flushBits( &pending, output );

  // ...
  fclose( input );
  fclose( output );
  freeWordList( wordList );
  return EXIT_SUCCESS;
}
