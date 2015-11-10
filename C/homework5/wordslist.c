/**
 This file provdes support for reading the word file and managing the word list.

 @file wordslist.c
 @author Janne Louise Ave (jfave)
 */

#include "wordlist.h"

#define UNSUCCESSFUL_EXIT_STATUS 100
#define MAX_LENGTH 22
#define HTAB 0x09
#define NLNE 0x0A
#define CRTRN 0x0D
#define SPACE 0x20
#define TILDE 0x7E

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
 Given a character, this function returns true if it's one of the 98
 valid characters that can appear in a word of the wordslist or in the text
 file being compressed.

 @param ch character that is being validated
 @return boolean value if the character is valid
 */
bool vaildChar( char ch )
{
    int check = ( int ) ch;
    if ( check == HTAB ||
         check == NLINE ||
         check == CRTRN ||
         check >= SPACE && check <= TILDE ) {
        return 1;
    } else {
        return 0;
    }
}

/**
 This function is responsible for building the word list. It reads words
 from the word from the given filen name "fname". After reading all the
 words from the word file, it adds single-character words for each of the
 98 valid characters.
 Sorts the word list lexicographically so that the index of each word is its code.
 */
WordList *readWordList( char const *fname )
{
    // should dynamically allocate the WordList structure
    // use the fields of this structure to allocate its words array as a
    // resizable array, growing the list of words as it needs room to
    // store more of them
    
    // use addWord to add a word to the WordList...
    File *wordFile = fopen( *fname, "r" );
    if ( input == NULL ) {
        fprintf( stderr, "Can't open word file: %s", fname );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    }
    struct WordList *wordList = ( WordList * )malloc( sizeof( WordList ));
    wordList->len = 0;
    wordList->capacity = 108; // 98 (first valid chars) + 10 extra words
    wordList->words = ( Word * )malloc( wordList->capacity * sizeof( Word ));
    // fill in the words array with the first 98 valid characters
    Word zero = ( char ) { 0x09 }; // horizontal tab
    Word one = ( char ) { 0x0A }; // newline
    Word two = ( char ) { 0x0D }; // carriage return
    words[0] = zero;
    words[1] = one;
    words[2] = two;
    int i = 0;
    for ( int j = 3; j < 98; j++ ) {
        words[j] = ( char ) { 0x20 + i };
        i++;
    }
    // ----------------------------------------------------------
    /**
    char a[ MAX_LENGTH ];
    char *line = &a;
    while ( fscanf( wordFile, "%22s", line ) ) {
        int numOfChars;
        Word *str = ( char )malloc( WORD_MAX * sizeof( char ));
        if ( sscanf( line, "%d ", &numOfChars ) != 1) { // the word def does not
            fprintf( stderr, "Invalid word file" );    // start with a decimanl int
            exit( UNSUCCESSFUL_EXIT_STATUS );
        }
        for ( int i = 0; i < numOfChars; i++) { // go through the rest of the line
            char ch; // account for each char
            sscanf( line, "%c", &ch ); // then check to make sure it is valid
            if ( validChar( ch ) == 0 ) { // char is not valid
                fprintf( stderr, "Invalid word file" );
                exit( UNSUCCESSFUL_EXIT_STATUS );
            }
        }
    }
     */
}

/** 
 Adds the string to the passed by reference WordList.

 @param word that needs to be added.
 @param wordList WordList structure to add the word to.
 */
void addWord( Word *word, int index, WordList *wordList );
    // ... if the wordList does not have any space
    // update the wordList using resize().

/**
 Resizes the given word list so it can add more words.

 @param wordList the WordList structure that needs to be
 expanded.
 @return the expanded WordList sturcture.
 */
WordList *resize( WordList *wordList );

/**
 This function returns the best code for representing
 the sequence of chracters at the start of a given string.

 @param wordList pointer to a WordList struct.
 @param str the string to find a code for.
 @return index of the longest word in the wordList
 the matches a prefix of the string.

int bestCode( WordList *wordList, char const *str )
{
    // Since the word list is sorted, we can use binary search to
    // quickly look for a matching word.
    // function to use:
    // void *bsearch( const void *key, <- prefix of str
    //                const void *base, <- pointer to the start of sorted array
    //                size_t nmemb, <- number of elements in the array
    //                size_t size, <- size of each element
    //                int (*compar)(const void *, const void *) <- use same func
    //                                                             from qsort()
    //               );
    
    // Call bsearch repeatedly with different prefixes of str.
    // Keep asking about shorter and shorter prefixes until it
    // finds a match.
    
    // When it finds a match, the return value of bsearch() will be pointer to a
    // word in the word list. Use ponter arithmetic to figure out the inddex of
    // this word in the word list and return it.
}
*/

/**
 Function free the memory for the given wordList, including the dynamically
 allocated list of words inside and the worldList structure itself.

 @param wordList the WordList to free.
 */
void freeWordList( WordList *wordList )
{
   // free any filed that has been malloced
    // then free the whole struct
    for ( int i = 0; i < wordList->capacity; i++) {
        free( wordList->words[i] );
    }
    free( wordList->words );
    free( wordList );
}
