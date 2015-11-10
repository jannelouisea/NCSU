#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ARGUMENTS 4
#define UNSUCCESSFUL_EXIT_STATUS 100
#define WORD_MAX 20
#define MAX_LENGTH 22
#define MAX_WORD_DEF 414
#define HTAB 0x09
#define NLNE 0x0A
#define CRTRN 0x0D
#define SPACE 0x20
#define TILDE 0x7E
#define HTABn 9
#define NLNEn 10
#define CRTRNn 13
#define SPACEn 32
#define TILDEn 126

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

int compareFunc( void const *a, void const *b )
{
    return strcmp( ( char * ) a, ( char * ) b );
}

int bestCode( WordList *w, char const *str )
{
    Word copy;
    int i = 2;
    copy[0] = *str;
    copy[1] = '\0';
    
    Word *temp;
    Word *index = bsearch( copy, w->words, w->len, sizeof( w->words[0] ), compareFunc );
    
    while ( (index != NULL) && (i <= WORD_MAX) && (*(str + i + 1) != '\0') ) {
        temp = index;
        for (int j = 0; j <= i; j++) {
            copy[j] = *(str + j);
        }
        copy[ i + 1 ] = '\0';
        i++;
        printf("copy: %s\n", copy );
        index = bsearch( copy, w->words, w->len, sizeof( w->words[0] ), compareFunc );
        printf("Index: %ld\n", temp - w->words);
    }
    
    return temp - w->words;
}

/**
 Function free the memory for the given wordList, including the dynamically
 allocated list of words inside and the worldList structure itself.
 
 @param wordList the WordList to free.
 */
void freeWordList( WordList *wordList )
{
    free( wordList->words );
    free( wordList );
}

void exitInvalid() {
    fprintf( stderr, "Invalid word file\n" );
    exit( UNSUCCESSFUL_EXIT_STATUS );
}

bool validChar( int c )
{
    if ( c == HTABn ||
         c == NLNEn ||
         c == CRTRN ||
       ( c >= SPACEn && c <= TILDEn )
        ) {
        return true;
    } else {
        return false;
    }
}

char *readFile( FILE *fp )
{
    int capacity = 20;
    char *buffer = malloc( capacity );
    int len = 0;
    char c = ' ';
    while ( fscanf( fp, "%c", &c ) == 1 ) {
        if ( validChar(c) == 0 ) {
            fprintf( stderr, "Invalid character code: %02x\n", c);
            free( buffer );
            exit( UNSUCCESSFUL_EXIT_STATUS );
        }
        if ( len >= capacity ) {
            buffer = realloc( buffer, capacity += 20 );
        }
        buffer[len] = c;
        len++;
    }
    buffer[len] = '\0';
    return buffer;
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
    w->capacity = 98;
    w->words = ( Word * )malloc( w->capacity * sizeof( Word )  );
    
    int charCount = -1;
    Word word;
    char ch;
    
    while ( fscanf( file, "%d%*c", &charCount ) != EOF ) {
        if ( charCount == -1 )
            exitInvalid();
        if ( charCount < 2 || charCount > 20 )
            exitInvalid();
        if ( w->len == 414 )
            exitInvalid();

        for ( int i = 0; i < charCount; i++ ) {
            fscanf( file, "%c", &ch );
            if ( !validChar(ch) )
                exitInvalid();
            word[ i ] = ch;
        }
        word[ charCount ] = '\0';
        
        if ( w->capacity == w->len ) {
            w->capacity += 100;
            w->words=( Word * )realloc( w->words, w->capacity * sizeof( Word ) );
        }
        
        for ( int i = 0; i < w->len; i++ ) {
            if ( !strcmp( w->words[ i ], word ) )
                exitInvalid();
        }
        
        strcpy( w->words[ w->len++ ], word );
        
        charCount = -1;
    }
    
    fclose( file );
    
    for ( int i = HTAB; i <= TILDE; i++ ){
        if ( validChar( i ) ) {
            char str[ 2 ];
            sprintf( str, "%c", i );
            strcpy( w->words[ w->len++ ], str );
        }
    }
    
    qsort( w->words, w->len, sizeof( w->words[ 0 ] ), compareFunc );
    
    return w;
}

int main(int argc, char *argv[]) {
    if ( argc > MAX_ARGUMENTS ) { // check how many arguments have been used
        fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [word_file.txt]\n" );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    } else {
        // create the wordList
        char *wordFile;
        if ( argc == 4 ) {
            wordFile = argv[ MAX_ARGUMENTS - 1 ];
        } else {
            wordFile = "words.txt"; // **************
        }
        // printf("Word file: %s\n", wordFile);
        WordList *wordList  = readWordList( wordFile );
        printf("wordlist->len: %d\n", wordList->len);
        printf( "---- word list sorted-----\n" );
        for ( int i = 0; i < wordList->len; i++ )
            printf( "%d == %s\n", i, wordList->words[ i ] );
        printf( "--------------------------\n" );
        printf("Created sorted wordlist\n");
        
        // create a buffer from the input.txt
        FILE *input = fopen( argv[1], "r" );
        if ( input == NULL ) {
            fprintf( stderr, "Can't open file: %s\n", argv[1] );
            freeWordList( wordList );
            exit( UNSUCCESSFUL_EXIT_STATUS );
        }
    
        char *buffer = readFile( input );
        printf("Created buffer\n");
        printf("---- input text ----\n");
        printf( "%s", buffer );
        printf("--------------------\n");

        int pos = 0;
        printf( "Is my problem here?\n" );
        while ( buffer[pos] ) {
            printf("Position in buffer: %c\n", buffer[pos] );
            int code = bestCode( wordList, buffer + pos );
            printf( "%d <- %s\n", code, wordList->words[ code ] );
            pos += strlen( wordList->words[ code ] );
        }
        
        fclose( input );
        freeWordList( wordList );
        free( buffer );
    }
}
