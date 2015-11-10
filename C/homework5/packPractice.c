#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// #include "wordlist.h"
// #include "bits.h"

#define MAX_ARGUMENTS 4
#define UNSUCCESSFUL_EXIT_STATUS 100
#define WORD_MAX 20
#define UNSUCCESSFUL_EXIT_STATUS 100
#define MAX_LENGTH 22
#define MAX_WORD_DEF 414
#define HTAB 0x09
#define NLNE 0x0A
#define CRTRN 0x0D
#define SPACE 0x20
#define TILDE 0x7E

/**
 Given a character, this function returns true if it's one of the 98
 valid characters that can appear in a word of the wordslist or in the text
 file being compressed.
 
 @param ch character that is being validated
 @return boolean value if the character is valid
bool isVaildChar( char ch, char *list )
{
    int length = sizeof(list)/sizeof(list[0]);
    for ( int i = 0; i < length; i++) {
        if ( list[i] == ch ) {
            return 1;
        }
    }
    return 0;
}
*/

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

void expand( WordList *wordList )
{
    wordList->capacity += 20;
    wordList->words = ( Word * )realloc( wordList->words, wordList->capacity );
}

void addWord( char *line, WordList *wordList, char *list )
{
    printf("Adding word\n");
	printf("%s\n", line);
	int numOfChars;
    if ( wordList->len >= wordList->capacity ) {
		printf("exapanded words array\n");
        expand( wordList );
    }
    if ( sscanf( line, "%d ", &numOfChars ) != 1 ) {
        fprintf( stderr, "Invalid word file\n" );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    } else {
        wordList->len++;
        for ( int i = 0; i < numOfChars; i++ ) {
            char ch;
            sscanf( line, "%c", &ch );
            //if ( isValidChar(ch, list) == 0 ) {
              //  wordList->len--;
                //fprintf( stderr, "Invalid word file\n" );
                //exit( UNSUCCESSFUL_EXIT_STATUS );
            //} else {
                wordList->words[wordList->len - 1][i] = ch;
            //}
        }
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
    int i = 0;
    for ( int j = 3; j < 98; j++ ) {
        validChars[j] = (char) SPACE + i;
        i++;
    }
    
    FILE *wordFile = fopen( fname, "r" );
    if ( wordFile == NULL ) {
        fprintf( stderr, "Can't open word file: %s", fname );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    }
    WordList *wordList = ( WordList * )malloc( sizeof( WordList ));
    wordList->len = 0;
    wordList->capacity = 108; // 98 (first valid chars) + 10 extra words
    wordList->words = ( Word * )malloc( wordList->capacity * sizeof( Word ));
    // fill in the words array with the first 98 valid characters
    for (int i = 0; i < 98; i++) {
        wordList->words[i][0] = validChars[i];
    }
    wordList->len = 98;
    int wordDefCount = 0;
    char a[MAX_LENGTH];
    char *line = &a[MAX_LENGTH];
	/**
    while ( fgets(line, 22, wordFile) != NULL ) {
        wordDefCount++;
		printf("Word Def Count: %d\n", wordDefCount);
        if ( wordDefCount > MAX_WORD_DEF ) {
            fprintf( stderr, "Invalid word file\n" );
            exit( UNSUCCESSFUL_EXIT_STATUS );
        }
        addWord( line, wordList, validChars );
    }
	*/
    return wordList;
}

/**
 Function free the memory for the given wordList, including the dynamically
 allocated list of words inside and the worldList structure itself.
 
 @param wordList the WordList to free.
 */
void freeWordList( WordList *wordList )
{
    /**
    printf( "freed 0\n");
    free( wordList->words[0] );
    printf( "freed 1\n");
    free( wordList->words[1] );
    printf( "wordlist length: %d\n", wordList->len );
    printf( "freeing each word array\n" );
    for ( int i = 3; i < wordList->len; i++) {
        printf( "word array %d\n", i);
        free( wordList->words[i] );
    }
     */
    printf( "freeing words field\n" );
    free( wordList->words );
    printf( "freeing wordList struct\n" );
    free( wordList );
}

int main(int argc, char *argv[] )
{
    if ( argc > MAX_ARGUMENTS ) { // check how many arguments have been used
        fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [word_file.txt]\n" );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    } else {
        char *wordFile; // check to see which wordFile string to use
        if ( argc == 4 ) {
            wordFile = argv[ MAX_ARGUMENTS - 1 ];
        } else {
            wordFile = "blah.txt"; // **************
        }
		printf("Word file: %s\n", wordFile);
        WordList *wordList = readWordList( wordFile );
        // Report the entire contents of the word list, once it's built.
        printf( "---- word list -----\n" );
        for ( int i = 0; i < wordList->len; i++ )
            printf( "%d == %s\n", i, wordList->words[ i ] );
        printf( "--------------------\n" );
        // FILE *input = fopen( argv[1], "r" );
        freeWordList( wordList );
    }
}
