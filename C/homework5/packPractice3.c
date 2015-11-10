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

int compareFunc(const void *a, const void *b)
{
    return strcmp(a,b);
}

int bestCode( WordList *w, char const *str )
{
    int numOfChars = 20;
    char copy[numOfChars] = { *str, };
    
    int i = 0;
    void *code = bsearch( copy, w, w->len, 21, compareFunc );
    void *temp;
    while ( code != NULL ) {
        temp = code;
        i++;
        copy[i] = *(str + i);
        
    }
}

bool validChar( int c )
{
    if ( c == HTABn ||
        c == NLNEn ||
        c == CRTRN ||
        ( c >= SPACEn && c <= TILDEn )
        ) {
        return 1;
    } else {
        return 0;
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

void addWord(int numOfChars, char const word[], WordList *w)
{
    if( numOfChars < 2 || numOfChars > 20 ) {
        fprintf( stderr, "Invalid word file\n" );
        freeWordList( w );
        exit(UNSUCCESSFUL_EXIT_STATUS);
    }
    
    w->len++;
    int index = w->len - 1;
    for (int i = 0; i < numOfChars ; i++) {
        w->words[index][i] = word[i];
    }
}

Word *readWordFile( FILE *wordFile )
{
    // printf("storing the words from the wordFile in a different array\n");
    int rest_len = 0;
    int rest_capacity = 414;
    printf("Malloc for rest\n");
	    	free( rest );
	    	free( str );
                fprintf( stderr, "Invalid word file\n");
                exit( UNSUCCESSFUL_EXIT_STATUS );
            } else {
                rest[index][i] = str[i];
            }
        }
        rest[index][numOfChars] = '\0';
    }
    
    /**
    printf( "-------- rest --------\n" );
    for ( int i = 0; i < rest_len; i++ )
        printf( "%d == %s\n", i, rest[ i ] );
    printf( "----------------------\n" );
     */
    qsort(rest, rest_len, 21 , compareFunc );
	 
    printf( "---- sorted rest -----\n" );
    for ( int i = 0; i < rest_len; i++ )
        printf( "%d == %s\n", i, rest[ i ] );
    printf( "----------------------\n" );

    free( str );

    return rest;
}

WordList *readWordList( char const *fname )
{
    printf("Reading the word file\n");
    FILE *wordFile = fopen( fname, "r" );	// FILE OPEN
    if ( wordFile == NULL ) {
        fprintf( stderr, "Can't open word file: %s", fname );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    }
 
    Word *commonWords = readWordFile( wordFile );
   
    printf("Creating the first 98 words in w->words array\n");
    WordList *w = ( WordList * )malloc( sizeof( WordList ));
    // printf("Setting variables\n");
    w->len = 0;
    w->capacity = 120;
    printf("Malloc for w->words\n");
    w->words = (Word *)malloc(w->capacity * sizeof(Word));
    
   // printf("Storing first\n");   
    w->words[0][0] = 0x09;
    w->words[0][1] = '\0';
   // printf("Storing second\n");
    w->words[1][0] = 0x0A;
    w->words[1][1] = '\0';
   // printf("Storing third\n");
    w->words[2][0] = 0x0D;
    w->words[2][1] = '\0';
    int j = 0;
    for (int i = 3; i < 98; i++) {
       // printf("Storing valid %d our of 98\n", i);
        w->words[i][0] = 0x20 + j;
        w->words[i][1] = '\0';
        j++;
    }
    w->len = 98;
   
    int k = 98;
    for ( int i = 0; commonWords[i]; i++) { // for every word in rest
	printf("%d, %s\n", k, commonWords[i]);
        w->len++;
        if ( w->len > w->capacity ) {
            printf("trying to realloc\n");
            w->words = realloc( w->words, w->capacity += 100 );
        }
        int word_length = strlen( commonWords[i] );
        for (int j = 0; j < word_length; j++) {
            w->words[k][j] = commonWords[i][j];
        }
        k++;
        if( w->capacity >= 512 ) {
            break;
        }
    }
    printf("Created sorted wordList\n");

   fclose( wordFile ); // FILE CLOSE
   free( commonWords );   // FREE MALLOC 1

   for( int i = 0; i < w->len; i++) {
       printf( "%d == %s\n", i, w->words[i] );  
   }

   return w;
}

int main(int argc, char *argv[]) {
    if ( argc > MAX_ARGUMENTS ) { // check how many arguments have been used
        fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [word_file.txt]\n" );
        exit( UNSUCCESSFUL_EXIT_STATUS );
    } else {
        // create the wordList
            break;
        }
    }
    printf("Created sorted wordList\n");

   fclose( wordFile ); // FILE CLOSE
   free( commonWords );   // FREE MALLOC 1

   for( int i = 0; i < w->len; i++) {
       printf( "%d == %s\n", i, w->words[i] );  
   }

   return w;
            break;
        }
    }
    printf("Created sorted wordList\n");

   fclose( wordFile ); // FILE CLOSE
   free( commonWords );   // FREE MALLOC 1

   for( int i = 0; i < w->len; i++) {
       printf( "%d == %s\n", i, w->words[i] );  
   }

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
      
        printf("find the best sequnce of codes for the whole buffer\n");
        int pos = 0;
        printf("buffer\n");
        printf("seg fault?\n");
        while ( buffer[pos] ) {
/**
            printf("%c", buffer[pos]);
            pos++;
*/
            int code = bestCode( wordList, buffer + pos );
            printf( "%d <- %s\n", code, wordList->words[ code ] );
            pos += strlen( wordList->words[ code ] );
        }
        
	fclose( input ); 
        freeWordList( wordList );
        free( buffer );
    }
}
