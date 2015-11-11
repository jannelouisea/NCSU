/**
 This file handles writing a code to write to the compressed
 file and reading character from the compressed file
 and getting a valid string from it.

 @author Janne Louise Ave (jfave)
 */

#include <stdio.h>

#include "bits.h"

/** Number of bits per byte.
 This isn't going to change, but it lets us give
 a good explanation instead of just the literal value, 8. */
#define BITS_PER_BYTE 8

/** Number of bits in each code written to or read from a file. */
#define BITS_PER_CODE 9

/** Number 7 */
#define NUM7 7

/** Number 6 */
#define NUM6 6

/** Number 5 */
#define NUM5 5

/** Number 4 */
#define NUM4 4

/** Number 3 */
#define NUM3 3

/** Number 2 */
#define NUM2 2

/** Binary mask that is all ones */
#define FULL_MASK 0xFF

/** Mask with nothing but 0s */
#define BLANK_MASK 0x00

/** Mask with 8 bits with last bit assigned to 1 */
#define MASK1 0x01

/** Mask with 8 bits with last bit assigned to 2 */
#define MASK2 0x7F

/** Mask with 8 bits with last bit assigned to 3 */
#define MASK3 0x3F

/** Mask with 8 bits with last bit assigned to 4 */
#define MASK4 0x1F

/** Mask with 8 bits with last bit assigned to 5 */
#define MASK5 0x0F

/** Mask with 8 bits with last bit assigned to 6 */
#define MASK6 0x07

/** Mask with 8 bits with last bit assigned to 7 */
#define MASK7 0x03

void flushBits( PendingBits *p, FILE *fp )
{
  fprintf( fp, "%c", p->bits );
}

void writeCode( int code, PendingBits *p, FILE *fp )
{
  char c;
  if ( 0 == p->bitCount ) {
    c = code & FULL_MASK;
    p->bits = code >> BITS_PER_BYTE;
    p->bitCount = 1;
    fprintf( fp, "%c", c );
  } else if ( p->bitCount == NUM7 ) {
    c = code << p->bitCount | p->bits;
    fprintf( fp, "%c", c );
    c = code >> ( BITS_PER_BYTE - p->bitCount );
    fprintf( fp, "%c", c );
    p->bits = BLANK_MASK;
    p->bitCount = 0;
  } else {
    c = code << p->bitCount | p->bits;
    p->bits = code >> ( BITS_PER_BYTE - p->bitCount );
    p->bitCount += 1;
    fprintf( fp, "%c", c );
  }
}

int readCode( PendingBits *p, FILE *fp )
{
  if ( p->bitCount == 0 ) {
    unsigned char l1;
    if ( fread( &l1, 1, 1, fp ) == 0 )
      return EOF;

    unsigned char l2;
    if ( fread( &l2, 1, 1, fp ) == 0 )
      return EOF;

    int index = ( ( l2 & MASK1 ) << BITS_PER_BYTE ) | l1;
    p->bits = l2 >> 1;
    p->bitCount = NUM7;
    return index;
  } else if ( p->bitCount == 1 ) {
    unsigned char l1;
    if ( fread( &l1, 1, 1, fp ) == 0 )
      return EOF;
    int index = l1 << 1 | p->bits;
    p->bitCount = 0;
    p->bits = 0;
    return index;
  } else {
    unsigned char l1;
    if ( fread( &l1, 1, 1, fp ) == 0 )
      return EOF;

    int mask = 0;
    if ( p->bitCount == NUM2 ) {
      mask = MASK2;
    } else if ( p->bitCount == NUM3 ) {
      mask = MASK3;
    } else if ( p->bitCount == NUM4 ) {
      mask = MASK4;
    } else if ( p->bitCount == NUM5 ) {
      mask = MASK5;
    } else if ( p->bitCount == NUM6 ) {
      mask = MASK6;
    } else {
      mask = MASK7;
    }

    int index = ( ( l1 & mask ) << p->bitCount ) | p->bits;
    p->bits = l1 >> ( BITS_PER_CODE - ( p->bitCount ) );
    p->bitCount -= 1;
    return index;
  }
}
