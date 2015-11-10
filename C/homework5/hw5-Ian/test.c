#include <stdio.h>
#include "wordlist.h"
#include "bits.h"

int main()
{
    WordList *list = readWordList( "words.txt" );
    printf( "-> List length: %d\n", list->len );
    for ( int i = 0; i < list->len; i++ ) {
        printf( "%d: %s\n", i, list->words[i] );
    }
    printf( "-> %d\n", bestCode( list, "need" ) );
    writeCode( bestCode( list, "need" ), NULL, NULL ); 
    freeWordList( list );
    return 0;
}
