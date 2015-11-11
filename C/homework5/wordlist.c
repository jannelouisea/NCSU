#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wordlist.h"

/** Maximum length of a word in wordlist. */
#define UNSUCCESSFUL_EXIT_STATUS 100

/** Amount to increment by */
#define AMOUNT 100

/** Number of valid chars */
#define VALID_CHARS 98

/** Max number of characters */
#define WORD_MAX 20

/** Min number of character */
#define WORD_MIN 2

/** Max length of the line */
#define MAX_LENGTH 22

/** Maximum word definitions */
#define MAX_WORD_DEF 414

/** Hex value for \t */
#define HTAB 0x09

/** Hex value for \n */
#define NLNE 0x0A

/** Hex value for \c */
#define CRTRN 0x0D

/** Hex value for ' ' */
#define SPACE 0x20

/** Hex value for a '~' */
#define TILDE 0x7E

void freeWordList( WordList *wordList )
{
  free( wordList->words );
  free( wordList );
}

int compareFunc( void const *a, void const *b )
{
  return strcmp( ( char * ) a, ( char * ) b );
}

int bestCode( WordList *w, char const *str )
{
  Word *index;
  int length = strlen( str );
  char copy[ length ];

  for ( int i = 0; i < length; i++ ) {
    strncpy( copy, str, length - i );
    copy[ length - i ] = '\0';
    index = bsearch( copy, w->words, w->len, sizeof( w->words[ 0 ] ), compareFunc );
    if ( index != NULL ) {
      break;
    }
  }

  return index - w->words;
}

bool validChar( int c )
{
  if ( c == HTAB ||
       c == NLNE ||
       c == CRTRN ||
     ( c >= SPACE && c <= TILDE ) ) {
    return true;
  } else {
    return false;
  }
}

WordList *readWordList( char const *fname )
{
  FILE *file = fopen( fname, "r" );
  if ( file == NULL ) {
    fprintf( stderr, "Can't open word file: %s\n", fname );
    exit( EXIT_FAILURE );
  }

  WordList *w = ( WordList * )malloc( sizeof( WordList ) );
  w->len = 0;
  w->capacity = VALID_CHARS;
  w->words = ( Word * )malloc( w->capacity * sizeof( Word )  );

  int charCount = -1;
  Word word;
  char ch;

  while ( fscanf( file, "%d%*c", &charCount ) != EOF ) {
    if ( charCount == -1 ) {
      freeWordList( w );
      fprintf( stderr, "Invalid word file\n" );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }
    if ( (charCount < WORD_MIN || charCount > WORD_MAX ) ) {
      freeWordList( w );
      fprintf( stderr, "Invalid word file\n" );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }
    if ( w->len == MAX_WORD_DEF ) {
      freeWordList( w );
      fprintf( stderr, "Invalid word file\n" );
      exit( UNSUCCESSFUL_EXIT_STATUS );
    }
    for ( int i = 0; i < charCount; i++ ) {
      fscanf( file, "%c", &ch );
      if ( !validChar( ch ) ) {
        freeWordList( w );
        fprintf( stderr, "Invalid word file\n" );
        exit( UNSUCCESSFUL_EXIT_STATUS );
      }
      word[ i ] = ch;
    }
    word[ charCount ] = '\0';

    if ( w->capacity == w->len ) {
      w->words=( Word * )realloc( w->words, ( w->capacity *= 2 ) * sizeof( Word ) );
    }

    for ( int i = 0; i < w->len; i++ ) {
      if ( !strcmp( w->words[ i ], word ) ) {
        freeWordList( w );
        fprintf( stderr, "Invalid word file\n" );
        exit( UNSUCCESSFUL_EXIT_STATUS );
      }
    }

    strcpy( w->words[ w->len++ ], word );
  }

  for ( int i = HTAB; i <= TILDE; i++ ){
    if ( validChar( i ) == true ) {
      char str[ 2 ];
      sprintf( str, "%c", i );
      strcpy( w->words[ w->len++ ], str );
    }
  }

  qsort( w->words, w->len, sizeof( w->words[ 0 ] ), compareFunc );

  fclose( file );

  return w;
}
