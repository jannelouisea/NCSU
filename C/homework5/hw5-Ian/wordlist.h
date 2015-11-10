#ifndef _WORDLIST_H_
#define _WORDLIST_H_

#include <stdbool.h>

/** Maximum length of a word in wordlist. */
#define WORD_MAX 20

/** Maximum length of a word in wordlist. */
#define WORD_MIN 2

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

#endif

/**
    Checks to see if the given character is one of the
    98 valid characters

    @param ch The characters whose validity is to be checked
    @return A boolean for the validity of the character
 */
bool validChar( char ch );

/**
    Creates a WordList given a filename from which to read
    additional Words
    
    @param *fname The filename as a String
    @return The constructed WordList
 */
WordList *readWordList( char const *fname );

/**
    Finds the index of the largest prefix of the string
    in the WordList

    @param *wordList The WordList, given by reference
    @param *str The String for whom the index of the largest prefix is to be found
    @return The index of the largest prefix
 */
int bestCode( WordList *wordList, char const *str );

/**
    Frees all dynamically allocated parts of the WordList,
    thus destroying it

    @param *wordList The WordList to free, given by reference
 */
void freeWordList( WordList *wordList );
