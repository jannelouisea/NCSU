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

#endif
