/*
 * algorithm.c
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */
#include "algorithm.h"

const int MOD_ADLER = 65521; // max prime less then 65536

/*
 * where data is the location of the data in physical memory and
 * len is the length of the data in bytes
**/
uint32_t adler32(unsigned char *data, size_t len) {
    uint32_t a = 1, b = 0;
    size_t index;

    /* Process each byte of the data in order */
    for (index = 0; index < len; ++index) {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}

uint32_t crc32(unsigned char *data, size_t len) {
  return 0;
}

void BMMakeSkip(const char *ptrn, size_t len, int *skip) {
  int i;

  for (i = 0; i < 256; i++) {
    skip[i] = (int) len;
  }

  while (0 != len) {
    skip[(unsigned char) *ptrn++] = (int) len--;
  }
}

/*
 * notice, the function not check the param, be carefully
 */
void BMMakeShift(const char *ptrn, size_t len, int *shift) {
  int *sptr = &shift[len - 1];
  const char *p1, *p2, *p3, *pptr = ptrn + len - 1;
  char c = *(ptrn + len - 1);

  *sptr = 1;
  pptr--;

  while (sptr-- != shift) {
    p1 = ptrn + len - 2;

    do {
      while ((p1 >= ptrn) && (*p1-- != c)) {
        ;
      }

      p2 = ptrn + len - 2;
      p3 = p1;

      while ((p3 >= ptrn) && (*p3-- == *p2--) && (p2 >= pptr)) {
        ;
      }
    } while ((p3 >= ptrn) && (p2 >= pptr));

    *sptr = (typeof(*sptr)) (shift + len - sptr + p2 - p3);
    pptr--;
  }
}

int BMSearch(const char *buf, const char *ptrn) {
  return BMMemSearch((const void *) buf, strlen(buf), (const void *) ptrn, strlen(ptrn));
}

int BMSearchEx(const char *buf, const char *ptrn, const int *skip, const int *shift) {
  return BMMemSearchEx(buf, strlen(buf), ptrn, strlen(ptrn), skip, shift);
}

int BMMemSearch(const void *buf, size_t blen, const void *ptrn, size_t plen) {
  int skip[256], shift[256];

  BMMakeSkip((const char *) ptrn, plen, skip);
  BMMakeShift((const char *) ptrn, plen, shift);

  return BMMemSearchEx(buf, blen, ptrn, plen, skip, shift);
}

int BMMemSearchEx(const void *buf, size_t blen, const void *ptrn, size_t plen, const int * skip, const int *shift) {
  int b_idx = (int) plen, p_idx, stride;
  int skip_stride, shift_stride;

  if (plen <= 0) {
    return -1;
  }

  while (b_idx <= (int) blen) {
    p_idx = (typeof(p_idx)) plen;
    while (((char *) buf)[--b_idx] == ((char *) ptrn)[--p_idx]) {
      if (b_idx < 0) {
        return 0;
      }
      if (p_idx == 0) {
        return b_idx;
      }
    }
    skip_stride = skip[((unsigned char *) buf)[b_idx]];
    shift_stride = shift[p_idx];
    stride = MAX(skip_stride, shift_stride);
    b_idx += MAX(stride, 2);
  }

  return -1;
}
