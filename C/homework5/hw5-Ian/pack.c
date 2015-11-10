#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wordlist.h"
#include "bits.h"

// ...
char *readFille( FILE *fp )
{
    int capacity = 5;
    int len = 0;
    char *input = ( char * )malloc( capacity );
    char readIn;
    while ( fscanf( fp "%c", &readIn ) == 1 ) {
        if ( len >= capacity )
            input = ( char * )realloc( input, capacity *= 2 );
        if ( isValidChar( readIn ) ) 
            input = [ len++ ] = readIn;
        else 
            fprintf( stderr, "Invalid character code: ", &readIn );
    }
    buffer[ len ] = '\0'

    fclose( fp );
    return input;
}

int main( int argc, char *argv[] )
{
  char *wordFile = "words.txt";

  // ...
  
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

  return EXIT_SUCCESS;
}
