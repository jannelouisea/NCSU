#include <stdio.h>
#include "bits.h"
void writeCode( int code, PendingBits *pending, FILE *fp )
{
    //printf( "%u\n", code );
    while ( code ) {
        if ( code & 1)
            printf("1");
        else
            printf("0");

        code >>= 1;
    }
    printf("\n");
}

void flushBits( PendingBits *pending, FILE *fp )
{

}

int readCode( PendingBits *pending, FILE *fp )
{
    return -1;
}
