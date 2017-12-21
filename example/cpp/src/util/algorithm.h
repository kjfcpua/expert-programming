/*
 * algorithm.h
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdint.h>

/*
 * from: https://en.wikipedia.org/wiki/Adler-32
 *
 * A = 1 + D1 + D2 + ... + Dn (mod 65521)
 * B = (1 + D1) + (1 + D1 + D2) + ... + (1 + D1 + D2 + ... + Dn) (mod 65521)
 *   = n×D1 + (n−1)×D2 + (n−2)×D3 + ... + Dn + n (mod 65521)

 * Adler-32(D) = B × 65536 + A
 *
 * The Adler-32 sum of the ASCII string "Wikipedia" would be calculated as follows:
 * A =  920 =  398 hex  (base 16)
 * B = 4582 = 11E6 hex
 * Output = 4,582 × 65,536 + 920 = 300286872 = 11E60398 hex
**/
uint32_t adler32(unsigned char *data, size_t len);

uint32_t crc32(unsigned char *data, size_t len);

/********************************BM(boyer-Moore)************************/
void BMMakeSkip(const char *ptrn, size_t len, int *skip);
void BMMakeShift(const char *ptrn, size_t len, int *shift);
int BMSearch(const char *buf, const char *ptrn);
int BMSearchEx(const char *buf, const char *ptrn, const int *skip,
               const int *shift);
int BMMemSearch(const void *buf, size_t blen, const void *ptrn, size_t plen);
int BMMemSearchEx(const void *buf, size_t blen, const void *ptrn, size_t plen,
                  const int *skip, const int *shift);

#endif /* ALGORITHM_H_ */
