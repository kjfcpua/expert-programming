/*
 * time.h
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */

#ifndef TIME_H_
#define TIME_H_

#define TIME_YEAR (1 << 1)
#define TIME_MON  (1 << 2)
#define TIME_DAY  (1 << 3)
#define TIME_SEC  (1 << 4)
#define TIME_MS   (1 << 5)
#define TIME_US   (1 << 6)

#define TIME_FMT_DATETIME (1)
#define TIME_FMT_DATE     (2)
#define TIME_FMT_TIME     (3)

WEAK_VAR(g_time, struct timeval);
WEAK_VAR(g_zone, struct timezone);

long TimeGet(int flag);
long TimeDiff(struct timeval *tv, struct timeval *base);
int TimeFormat(char *str, size_t size, int format, struct timeval *tv);

#endif /* TIME_H_ */
