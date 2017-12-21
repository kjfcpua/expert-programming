/*
 * time.c
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */

#include "time.h"

long TimeGet(int flag) {
  struct timeval tv;
  long tm = -1;

  gettimeofday(&tv, NULL);

  switch (flag) {
    case TIME_SEC:
      tm = tv.tv_sec;
      break;
    case TIME_MS:
      tm = (tv.tv_sec % 1000) * 1000 + tv.tv_usec / 1000;
      break;
    case TIME_US:
      tm = tv.tv_usec;
      break;
  }

  return tm;
}

long TimeDiff(struct timeval *tv, struct timeval *base) {
  return (tv->tv_sec - base->tv_sec) * 1000 +
         (tv->tv_usec - base->tv_usec) / 1000;
}

int TimeFormat(char *str, size_t size, int format, struct timeval *tv) {
  struct tm *ptm = gmtime(&tv->tv_sec);
  int num = -1;
  switch (format) {
    case TIME_FMT_DATETIME:
      num = snprintf(str, size, "%04d-%02d-%02d %02d:%02d:%02d",
                     (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday,
                     ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
      break;
  }
  return num;
}
