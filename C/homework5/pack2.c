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
/** Number of bits per byte.  This isn't going to change, but it lets us give
 a good explanation instead of just the literal value, 8. */
#define BITS_PER_BYTE 8

/** Number of bits in each code written to or read from a file. */
#define BITS_PER_CODE 9

/** Buffer space for up to 8 bits that we're not finished processing.
 We have to read/write files one or more bytes at a time, but we
 need to access this data 9 bits at a time.  While writing a file,
 this buffer temporarily holds bits waiting to be written to the
 file (waiting for a full group of 8), or, while reading, bits
 we've already read but that the client code doesn't need yet. */
typedef struct {
    /** Storage for the unprocessed bits. */
    unsigned char bits;
    
    /** Number of unprocessed bits stored in this buffer. */
    int bitCount;
} PendingBits;

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

void flushbits( PendingBits *p, FILE *fp )
{
    fprintf( fp, "%c", p->bits );
}

/** Write the 9 low-order bits from code to the given file.
 @param code bits to write out, a value betteen 0 and 2^9 - 1.
 @param pending pointer to storage for unwritten bits left over
 from the previous call to writeCode().  After this call, any bits
 that partially fill the next byte will be left in the pending
 struct, to be written in the next call.
 @param fp file we're writing to, opened for writing.
 */
void writeCode( int code, PendingBits *p, FILE *fp )
{
    char c;
    if ( p->bitCount == 0 ) {
        c = code & 0xFF;
        p->bits = code >> BITS_PER_BYTE;
        p->bitCount = 1;
        fprintf( fp, "%c", c );
    } else if ( p->bitCount == 7 ) {
        c = code << p->bitCount | p->bits;
        fprintf( fp, "%c", c );
        c = code >> ( BITS_PER_BYTE - p->bitCount );
        fprintf( fp, "%c", c );
        p->bits = 0x00;
        p->bitCount = 0;
    } else {
        c = code << p->bitCount | p->bits;
        p->bits = code >> ( BITS_PER_BYTE - p->bitCount );
        p->bitCOunt += 1;
        fprintf( fp, "%c", c );
    }
}

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
    
    if ( *(str + 1) == '\0' ) {
        temp = bsearch( copy, w->words, w->len, sizeof( w->words[0] ), compareFunc );
    } else {
        while ( (index != NULL) && (i <= WORD_MAX) ) {
            temp = index;
            for (int j = 0; j <= i; j++) {
                copy[j] = *(str + j);
            }
            copy[ i + 1 ] = '\0';
            i++;
            index = bsearch( copy, w->words, w->len, sizeof( w->words[0] ), compareFunc );
        }
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
            wordFile = "words.txt";
        }
        WordList *wordList  = readWordList( wordFile );
        printf("wordlist->len: %d\n", wordList->len);
        printf( "---- word list sorted-----\n" );
        for ( int i = 0; i < wordList->len; i++ )
            printf( "%d == %s\n", i, wordList->words[ i ] );
        printf( "--------------------------\n" );
        
        // create a buffer from the input.txt
        FILE *input = fopen( argv[1], "r" );
        if ( input == NULL ) {
            fprintf( stderr, "Can't open file: %s\n", argv[1] );
            freeWordList( wordList );
            exit( UNSUCCESSFUL_EXIT_STATUS );
        }
    
        char *buffer = readFile( input );
        printf("---- input text ----\n");
        printf( "%s", buffer );
        printf("--------------------\n");
        
        // FILE *output = fopen( argv[2], "w" );
        FILE *output = stdout;
        if ( input == NULL ) {
            fprintf( stderr, "Can't open file: %s\n", argv[2] );
            freeWordList( wordList );
            exit( UNSUCCESSFUL_EXIT_STATUS );
        }

        int pos = 0;
        PendingBits p = { 0, 0 };
        while ( buffer[pos] ) {
            int code = bestCode( wordList, buffer + pos );
            printf( "%d <- %s\n", code, wordList->words[ code ] );
            writeCode( code, &p, output );
            pos += strlen( wordList->words[ code ] );
        }
        
        flushbits( p, output );
        
        fclose( input );
        freeWordList( wordList );
        free( buffer );
    }
}
