/**
  @file bits.c
  @author Erin Snider (eesnider)
  Writes and reads the hex code for compressing and decompressing
  */
#include <stdio.h>
#include "bits.h"

/** Write the 9 low-order bits from code to the given file.  
    @param code bits to write out, a value betteen 0 and 2^9 - 1.
    @param pending pointer to storage for unwritten bits left over
    from the previous call to writeCode().  After this call, any bits
    that partially fill the next byte will be left in the pending
    struct, to be written in the next call.
    @param fp file we're writing to, opened for writing.
*/
void writeCode( int code, PendingBits *pending, FILE *fp )
{
  if ( pending -> bitCount == 0 ) {
    unsigned char c = code & 0xFF;
    pending -> bits &= ( ( code & 0x100 ) >> 8 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c );
  } else if ( pending -> bitCount == 1 ) {
    unsigned char c = 0x00;
    c &= pending -> bits;
    pending -> bits &= ( ( code & 0x180 ) >> 7 );
    c |= ( ( code & 0x7F ) << 1 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c );
  } else if ( pending -> bitCount == 2 ) {
    unsigned char c = 0x00;
    c &= pending -> bits;
    pending -> bits &= ( ( code & 0x1C0 ) >> 6 );
    c |= ( ( code & 0x3F ) << 2 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c );
  } else if ( pending -> bitCount == 3 ) {
    unsigned char c = 0x00;
    c &= pending -> bits;
    pending -> bits &= ( ( code & 0x1E0 ) >> 5 );
    c |= ( ( code & 0x1F ) << 3 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c );
  } else if ( pending -> bitCount == 4 ) {
    unsigned char c = 0x00;
    c &= pending -> bits;
    pending -> bits &= ( ( code & 0x1F0 ) >> 4 );
    c |= ( ( code & 0x0F ) << 4 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c );
  } else if ( pending -> bitCount == 5 ) {
    unsigned char c = 0x00;
    c &= pending -> bits;
    pending -> bits &= ( ( code & 0x1F8 ) >> 3 );
    c |= ( ( code & 0x07 ) << 5 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c);
  } else if ( pending -> bitCount == 6 ) {
    unsigned char c = 0x00;
    c &= pending -> bits;
    pending -> bits &= ( ( code & 0x1FC ) >> 2 );
    c |= ( ( code & 0x03 ) << 6 );
    pending -> bitCount += 1;
    fprintf( fp, "%u", c ); 
  } else {
    unsigned char c = code & 0xFF;
    unsigned char d = 0x00;
    d &= pending -> bits;
    d |= ( ( code & 0x01 ) << 7 );
    fprintf( fp, "%u", c );
    fprintf( fp, "%u", d );
    pending -> bitCount = 0;
    pending -> bits &= 0x00;
  }
}

/** If there are any bits buffered in pending, write them out to the
    given file in the low-order bit positions of a byte, leaving zeros
    in the high-order bits.
    @param pending pointer to storage for unwritten bits left over
    from the most recent call to writeCode().
    @param fp file these bits are to be written to, opened for writing.
*/
void flushBits( PendingBits *pending, FILE *fp )
{
  fprintf( fp, "%u", pending -> bits );
}

/** Read and return the next 9-bit code from the given file.
    @param pending pointer to storage for left-over bits read during
    the last call to readCode().
    @param fp file bits are being read from, opened for reading.
    @return value of the 9-bit code read in, or -1 if we reach the
    end-of-file before getting 9 bits.
*/
int readCode( PendingBits *pending, FILE *fp ) {
  unsigned char c;
  int i = 0;
  fscanf( fp, "%hhu", &c );
  if ( pending -> bitCount == 0 ) {
    unsigned char d;
    fscanf( fp, "%hhu", &d );
    i &= c;
    i |= ( ( d & 0x01 ) << 8 );
    pending -> bits = ( ( d & 0xFE ) >> 1 );
    pending -> bitCount = 7;
    return i;
  } else if ( pending -> bitCount == 7 ) {
    i &= pending -> bits;
    i |= ( ( c & 0x03 ) << 7 );
    pending -> bits = ( ( c & 0xFC ) >> 2 );
    pending -> bitCount = 6;
    return i;
  } else if ( pending -> bitCount == 6 ) {
    i &= pending -> bits;
    i |= ( ( c & 0x07 ) << 6 );
    pending -> bits = ( ( c & 0xF8 ) >> 3 );
    pending -> bitCount = 5;
    return i;
  } else if ( pending -> bitCount == 5 ) {
    i &= pending -> bits;
    i |= ( ( c & 0x0F ) << 5 );
    pending -> bits = ( ( c & 0xF0 ) >> 4 );
    pending -> bitCount = 4;
    return i;
  } else if ( pending -> bitCount == 4 ) {
    i &= pending -> bits;
    i |= ( ( c & 0x1F ) << 4 );
    pending -> bits = ( ( c & 0xE0 ) >> 5 );
    pending -> bitCount = 3;
    return i;
  } else if ( pending -> bitCount == 3 ) {
    i &= pending -> bits;
    i|= ( ( c & 0x1F ) << 3 );
    pending -> bits = ( ( c & 0xC0 ) >> 6 );
    pending -> bitCount = 2;
    return i;
  } else if ( pending -> bitCount == 2 ) {
    i &= pending -> bits;
    i |= ( ( c & 0x3F ) << 2 );
    pending -> bits = ( ( c & 0x80 ) >> 7 );
    pending -> bitCount = 1;
    return i;
  } else {
    i &= pending -> bits;
    i|= ( ( c & 0x7F ) << 1 );
    pending -> bits = 0x00;
    pending -> bitCount = 0;
    return i;
  }
}
