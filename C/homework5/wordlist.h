/**
 This is a header file for wordist.c
 
 @author Janne Louise Ave (jfave)
 */

#ifndef _WORDLIST_H_
#define _WORDLIST_H_

#include <stdbool.h>

/** Maximum length of a word in wordlist. */
#define WORD_MAX 20

/** Word type, used to store elements of the word list,
    with room for a word of up to 20 characters. */
typedef char Word[ WORD_MAX + 1 ];

/** Representation for the whole wordlist.  It contains
    the list of words as a resizable, dynamically allocated
    array, along with supporting fields for resizing. */
typedef struct {
  /** Number of words in the wordlist. */
  int len;

  /** Capacity of the wordlist, so we can know when we need to resize. */
  int capacity;

  /** List of words.  Should be sorted lexicographically once the word list
      has been read in. */
  Word *words;
} WordList;

/**
 Function free the memory for the given wordList, including the dynamically
 allocated list of words inside and the worldList structure itself.
 
 @param wordList the WordList to free.
 */
void freeWordList( WordList *wordList );

/**
 Function compares two strings.

 @param a pointer to a string.
 @param b pointer to a string.
 @return an int value signifying if they are equal or not.
 */
int compareFunc( void const *a, void const *b );

/**
 This function returns an index in the wordList,
 to the best and longest string that matches str.

 @param w WordList.
 @param str pointer in the string buffer.
 @return index to the best and longest value in WordList w.
 */
int bestCode( WordList *w, char const *str );

/**
 This function determines if a character is valid.
 
 @param c char to be checked.
 @return true if the char is valid and false
         if it isnt.
 */
bool validChar( int c );

/**
 Creates a WordList structure. The words fields is
 filled in with values from the file opened
 with fname.
 
 @param fname name of the file.
 */
WordList *readWordList( char const *fname );

#endif
