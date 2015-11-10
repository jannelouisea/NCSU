/**
 This file is responsible for reading and writing sequences fo bits.

 @file bits.c
 @author Janne Louise Ave (jfave)
 */

#include "bits.h"

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

/** Write the 9 low-order bits from code to the given file.
 @param code bits to write out, a value betteen 0 and 2^9 - 1.
 @param pending pointer to storage for unwritten bits left over
 from the previous call to writeCode().  After this call, any bits
 that partially fill the next byte will be left in the pending
 struct, to be written in the next call.
 @param fp file we're writing to, opened for writing.
 */
void writeCode( int code, PendingBits *pending, FILE *fp );

/** If there are any bits buffered in pending, write them out to the
 given file in the low-order bit positions of a byte, leaving zeros
 in the high-order bits.
 @param pending pointer to storage for unwritten bits left over
 from the most recent call to writeCode().
 @param fp file these bits are to be written to, opened for writing.
 */

// use this function after writing all the bits in the words in the input file
void flushBits( PendingBits *pending, FILE *fp );

/** Read and return the next 9-bit code from the given file.
 @param pending pointer to storage for left-over bits read during
 the last call to readCode().
 @param fp file bits are being read from, opened for reading.
 @return value of the 9-bit code read in, or -1 if we reach the
 end-of-file before getting 9 bits.
 */

// returns a 9-bit sequence code that we need to turn into characters
// used by unpack.c
int readCode( PendingBits *pending, FILE *fp );
