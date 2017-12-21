/*
 * util.h
 *
 *  Created on: Aug 15, 2017
 *      Author: hanchen
 */

#ifndef UTIL_H_
#define UTIL_H_

/* add type support, e.g. OFFSETOF(*pst, m) */
#if !defined OFFSETOF
#ifdef __cplusplus
#define OFFSETOF(st, m) (((size_t)(&((typeof(st) *)1)->m)) - 1)
#else
#define OFFSETOF(st, m) (size_t)(&((typeof(st) *)0)->m)
#endif
#endif

/* the size of the structure's element */
// #define SZSTMEM(st, m) sizeof(((typeof(st) *) 0)->m)
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))

/* the size between pointers */
#define OFFSET(p, b) ((ssize_t)((size_t) (p) - (size_t) (b)))

/* if the p is arr, use like OFFOF((void *) arr, n) */
#ifdef __cplusplus
#define OFFOF(p, n) ((typeof(p)) ((char *) (p) + (n)))
#else
#define OFFOF(p, n) ((typeof(p)) ((void *) (p) + (n)))
#endif

/* to expand macro value first */
#define _MACRO_STR(v)   (#v)
#define MACRO_STR(v)    _MACRO_STR(v)

/*
#if !defined(STRNCPY)
#define STRNCPY(_d, _s, _n) strncpy_v(_d, _s, _n)
#endif
*/

#if !defined(STR_EMPTY)
#define STR_EMPTY(str)        ((str == NULL) || (str[0] == '\0'))
#endif

#if !defined(STR_CLEAR)
#define STR_CLEAR(str)        ((str != NULL) && (str[0] = '\0'))
#endif

#define WEAK_VAR(name, type)           __attribute__((weak)) type name
#define WEAK_VAR_INIT(name, type, val) __attribute__((weak)) type name = val

#if defined(__APPLE__)
#define WEAK_FUNC_0(name, prx, rt)                 rt __attribute__((weak)) name()
#define WEAK_FUNC_1(name, prx, rt, p1)             rt __attribute__((weak)) name(p1)
#define WEAK_FUNC_2(name, prx, rt, p1, p2)         rt __attribute__((weak)) name(p1, p2)
#define WEAK_FUNC_3(name, prx, rt, p1, p2, p3)     rt __attribute__((weak)) name(p1, p2, p3)
#define WEAK_FUNC_4(name, prx, rt, p1, p2, p3, p4) rt __attribute__((weak)) name(p1, p2, p3, p4)
#else
#define WEAK_FUNC_0(name, prx, rt)                 rt prx##name()
#define WEAK_FUNC_1(name, prx, rt, p1)             rt prx##name(p1)
#define WEAK_FUNC_2(name, prx, rt, p1, p2)         rt prx##name(p1, p2)
#define WEAK_FUNC_3(name, prx, rt, p1, p2, p3)     rt prx##name(p1, p2, p3)
#define WEAK_FUNC_4(name, prx, rt, p1, p2, p3, p4) rt prx##name(p1, p2, p3, p4)
#endif

#if defined(__linux__)
#define ALIAS_FUNC_0(an, name, rt)                 rt an()               __attribute__((weak, alias(#name)))
#define ALIAS_FUNC_1(an, name, rt, p1)             rt an(p1)             __attribute__((weak, alias(#name)))
#define ALIAS_FUNC_2(an, name, rt, p1, p2)         rt an(p1, p2)         __attribute__((weak, alias(#name)))
#define ALIAS_FUNC_3(an, name, rt, p1, p2, p3)     rt an(p1, p2, p3)     __attribute__((weak, alias(#name)))
#define ALIAS_FUNC_4(an, name, rt, p1, p2, p3, p4) rt an(p1, p2, p3, p4) __attribute__((weak, alias(#name)))
#elif defined(__CYGWIN__)
#define ALIAS_FUNC_0(an, name, rt)                 rt an()               __attribute__((alias(#name)))
#define ALIAS_FUNC_1(an, name, rt, p1)             rt an(p1)             __attribute__((alias(#name)))
#define ALIAS_FUNC_2(an, name, rt, p1, p2)         rt an(p1, p2)         __attribute__((alias(#name)))
#define ALIAS_FUNC_3(an, name, rt, p1, p2, p3)     rt an(p1, p2, p3)     __attribute__((alias(#name)))
#define ALIAS_FUNC_4(an, name, rt, p1, p2, p3, p4) rt an(p1, p2, p3, p4) __attribute__((alias(#name)))
#elif defined(__APPLE__)
#define ALIAS_FUNC_0(an, name, rt)
#define ALIAS_FUNC_1(an, name, rt, p1)
#define ALIAS_FUNC_2(an, name, rt, p1, p2)
#define ALIAS_FUNC_3(an, name, rt, p1, p2, p3)
#define ALIAS_FUNC_4(an, name, rt, p1, p2, p3, p4)
#else
#define ALIAS_FUNC_0(an, name, rt)
#define ALIAS_FUNC_1(an, name, rt, p1)
#define ALIAS_FUNC_2(an, name, rt, p1, p2)
#define ALIAS_FUNC_3(an, name, rt, p1, p2, p3)
#define ALIAS_FUNC_4(an, name, rt, p1, p2, p3, p4)
#endif

#ifndef MIN
#define MIN(m, n) \
({ \
  typeof(m) _m = (m); \
  typeof(n) _n = (n); \
  (_m < _n) ? _m : _n; \
})
#endif

#ifndef MAX
#define MAX(m, n) \
({ \
  typeof(m) _m = (m); \
  typeof(n) _n = (n); \
  (_m > _n) ? _m : _n; \
})
#endif

 #define VAL_MIN(n) \
({ \
  typeof(n) _min = 0; \
  if (ISTYPE(n, int)) { \
    _min = (typeof(_min)) INT_MIN; \
  } else if (ISTYPE(n, short)) { \
    _min = (typeof(_min)) SHRT_MIN; \
  } else if (ISTYPE(n, char)) { \
    _min = (typeof(_min)) SCHAR_MIN; \
  } else if (ISTYPE(n, long)) { \
    _min = (typeof(_min)) LONG_MIN; \
  } else { \
    BUILD_BUG_ON(ISTYPE(n, int)); \
  } \
  _min; \
})

#define VAL_MAX(n) \
({ \
  typeof(n) _max = 0; \
  if (ISTYPE(n, int)) { \
    _max = (typeof(_max)) INT_MAX; \
  } else if (ISTYPE(n, unsigned)) { \
    _max = (typeof(_max)) UINT_MAX; \
  } else if (ISTYPE(n, short)) { \
    _max = (typeof(_max)) SHRT_MAX; \
  } else if (ISTYPE(n, unsigned short)) { \
    _max = (typeof(_max)) USHRT_MAX; \
  } else if (ISTYPE(n, char)) { \
    _max = (typeof(_max)) SCHAR_MAX; \
  } else if (ISTYPE(n, unsigned char)) { \
    _max = (typeof(_max)) UCHAR_MAX; \
  } else if (ISTYPE(n, long)) { \
    _max = (typeof(_max)) LONG_MAX; \
  } else if (ISTYPE(n, unsigned long)) { \
    _max = (typeof(_max)) ULONG_MAX; \
  } else { \
    BUILD_BUG_ON(ISTYPE(n, int)); \
  } \
    _max; \
})

#ifndef SWAP
#define SWAP(m, n) \
({ \
  typeof(m) _t; \
  _t = (m); \
  (m) = (n); \
  (n) = _t;\
})
#endif

#define ABS(v) \
({ \
  typeof(v) _v = (v); \
  if (_v < 0) { \
    _v = -_v; \
  } \
  _v; \
})

#ifndef BITSET
#define BITSET(b, i, v) (((b) & ~(1UL << (i))) | ((!!(v)) << (i)))
#endif

#ifndef BITGET
#define BITGET(b, i)    (!!((b) & (1UL << (i))))
#endif

/* map, size, index, offset, value */
#define BITMAP_SET(m, s, i, o, v) \
({ \
    uint8_t *_m = (uint8_t *) m; \
    typeof(s) _s = s; \
    typeof(i) _i = i; \
    typeof(o) _o = o; \
    typeof(v) _v = v; \
    off_t _t = _s * _i + _o; \
    _m = OFFOF(_m, _t / 8); \
  *_m = BITSET(*_m, _t % 8, !!(_v)); \
  _v; \
})

/* map, size, index, offset, value */
#define BITMAP_GET(m, s, i, o) \
({ \
    uint8_t *_m = (uint8_t *) m; \
    typeof(s) _s = s; \
    typeof(i) _i = i; \
    typeof(o) _o = o; \
    int _v = 0; \
    off_t _t = _s * _i + _o; \
    _m = OFFOF(_m, _t / 8); \
  _v = BITGET(*_m, _t % 8); \
    _v; \
})

#define CONV1(conv, var) var = conv(var)
#define CONV2(conv, v1, v2) \
  do { \
    CONV1(conv, v1); \
    CONV1(conv, v2); \
  } while (0)
#define CONV3(conv, v1, v2, v3) \
  do { \
    CONV2(conv, v1, v2); \
    CONV1(conv, v3); \
  } while (0)
#define CONV4(conv, v1, v2, v3, v4) \
  do { \
    CONV2(conv, v1, v2); \
    CONV2(conv, v3, v4); \
  } while (0)

/* navite type compare function */
#define N_CMP(t, m, n) \
({ \
  typeof(t) _m = (typeof(t)) (m); \
  typeof(t) _n = (typeof(t)) (n); \
  (_m == _n) ? 0 : ((_m < _n) ? -1 : 1); \
})

#define NP_CMP(t, m, n) \
({ \
  typeof(t) _m = *((typeof(t) *) (m)); \
  typeof(t) _n = *((typeof(t) *) (n)); \
  (_m == _n) ? 0 : ((_m < _n) ? -1 : 1); \
})

#define NP_HASH(t, v) \
({ \
  unsigned long _h = *v; \
  _h; \
})

#define ADD_ANY(p, x, v) \
({ \
  int ret = -1, max = -1; \
  if (x != NULL) { \
    max = *x; \
  } \
  if (p != NULL && ((max < 0) || (max >= (int) sizeof(v)))) { \
    *(typeof(v) *) p = v; \
    p = OFFOF(p, sizeof(v)); \
    if (x != NULL) { \
      max -= sizeof(v); \
      *x = max; \
    } \
    ret = 0; \
  } \
  ret; \
})
#define ADD_BYTE(p, x, v)    ADD_ANY(p, x, (uint8_t)  v)
#define ADD_WORD(p, x, v)    ADD_ANY(p, x, (uint16_t) v)
#define ADD_DWORD(p, x, v)   ADD_ANY(p, x, (uint32_t) v)
#define ADD_DDWORD(p, x, v)  ADD_ANY(p, x, (uint64_t) v)
#define ADD_BUF(p, x, b, n) \
({ \
  int ret = -1, max = -1; \
  if (x != NULL) { \
    max = *x; \
  } \
  if (p != NULL && ((max < 0) || (max >= (int) n))) { \
    memcpy(p, b, n); \
    p = OFFOF(p, n); \
    if (x != NULL) { \
      max -= n; \
      *x = max; \
    } \
    ret = 0; \
  } \
  ret; \
})

#define GET_ANY_EX(r, p, x, t, c) \
({ \
  int ret = -1, max = -1; \
  typeof(c) *cv = NULL; \
  cv = (typeof(cv)) c; \
  if (x != NULL) { \
    max = *x; \
  } \
  if (p != NULL && ((max < 0) || (max >= (int) sizeof(t)))) { \
    if (r != NULL) { \
      *r = *(typeof(t) *) p; \
      if (cv != NULL) { \
        *r = (*cv)(*r); \
      } \
    } \
    p = OFFOF(p, sizeof(t)); \
    if (x != NULL) { \
      max -= sizeof(t); \
      *x = max; \
    } \
    ret = 0; \
  } \
  ret; \
})

#define GET_ANY(r, p, x)      GET_ANY_EX(r, p, x, typeof(*r), ((typeof(*r)(*)(typeof(*r))) NULL))
#define GET_BYTE(r, p, x)     GET_ANY_EX(r, p, x, uint8_t, ((uint8_t (*)(uint8_t)) NULL))
#define GET_WORD(r, p, x)     GET_ANY_EX(r, p, x, uint16_t, ((uint16_t (*)(uint16_t)) NULL))
#define GET_N_WORD(r, p, x)   GET_ANY_EX(r, p, x, uint16_t, ntohs)
#define GET_DWORD(r, p, x)    GET_ANY_EX(r, p, x, uint32_t, ((uint32_t (*)(uint32_t)) NULL))
#define GET_N_DWORD(r, p, x)  GET_ANY_EX(r, p, x, uint32_t, ntohl)
#define GET_DDWORD(r, p, x)   GET_ANY_EX(r, p, x, uint64_t, ((uint64_t (*)(uint64_t)) NULL))
#define GET_N_DDWORD(r, p, x) GET_ANY_EX(r, p, x, uint64_t, ntohx)

/*
 * memcpy(r != NULL ? r : p, p, n);
 * the avoid warning: null argument where non-null required
**/
#define GET_BUF(r, p, x, n) \
({ \
  int ret = -1, max = -1; \
  if (x != NULL) { \
    max = *x; \
  } \
  if (p != NULL && ((max < 0) || (max >= n))) { \
    memmove(r != NULL ? r : p, p, n); \
    p = OFFOF(p, n); \
    if (x != NULL) { \
      max -= n; \
      *x = max; \
    } \
    ret = 0; \
  } \
  ret; \
})

#define _DO_LOG_MORE(fp, fmt, args...) do { struct timeval _tv; struct tm _tm; char _ts[32]; gettimeofday(&_tv, NULL); _tm = *localtime(&_tv.tv_sec); strftime(_ts, sizeof(_ts), "%Y-%m-%d %H:%M:%S", &_tm); fprintf(fp, "[%s.%06ld] %s:%d(%s): "fmt"\n", _ts, (long) _tv.tv_usec, __FILE__, __LINE__, __func__, ##args); } while (0)
#define _DO_LOG_LESS(fp, fmt, args...) do { fprintf(fp, ""fmt"\n", ##args); } while (0)
#define _DO_LOG_NORM(fp, fmt, args...) do { struct timeval _tv; struct tm _tm; char _ts[32]; gettimeofday(&_tv, NULL); _tm = *localtime(&_tv.tv_sec); strftime(_ts, sizeof(_ts), "%Y-%m-%d %H:%M:%S", &_tm); fprintf(fp, "[%s.%06ld]: "fmt"\n", _ts, (long) _tv.tv_usec, ##args); } while (0)

#if defined(_DEBUG_MORE)
#define _DO_DEBUG(fp, fmt, args...) do { _DO_LOG_MORE(fp, fmt, ##args); } while (0)
#elif defined(_DEBUG_LESS)
#define _DO_DEBUG(fp, fmt, args...) do { _DO_LOG_LESS(fp, fmt, ##args); } while (0)
#elif defined(_DEBUG)
#define _DO_DEBUG(fp, fmt, args...) do { _DO_LOG_NORM(fp, fmt, ##args); } while (0)
#else
#define _DO_DEBUG(fp, fmt, args...) do { } while (0)
#endif // _DEBUG_MORE

#if !defined(DEBUGS)
#ifdef _DEBUG_FILE // _DEBUG_FILE
#define DEBUGS(fmt, args...) do { FILE *_f = fopen(MACRO_STR(_DEBUG_FILE), "a"); if (_f != NULL) { _DO_DEBUG(_f, fmt, ##args); fclose(_f); } } while (0)
#define DEBUGXXD(buf, len) do { char str[256]; int i = 0; FILE *_f = fopen(MACRO_STR(_DEBUG_FILE), "a"); if (_f != NULL) { for (i = 0; i < len; i += 16) { snprintfxxd(str, sizeof(str), OFFOF(buf, i), 16); _DO_LOG_LESS(stdout, "%s", str); } fclose(_f); } } while (0)
#else // _DEBUG_FILE
#define DEBUGS(fmt, args...) do { _DO_DEBUG(stdout, fmt, ##args); } while (0)
#define DEBUGXXD(buf, len) do { char str[256]; int i = 0; for (i = 0; i < len; i += 16) { snprintfxxd(str, sizeof(str), OFFOF(buf, i), 16); _DO_LOG_LESS(stdout, "%s", str); } } while (0)
#endif // _DEBUG_FILE
#endif // DEBUG

typedef struct {
  int key;
  int value;
} int_pair_t;

typedef struct {
  int  key;
  void *value;
} int_ptr_pair_t;

typedef struct {
  uint32_t key;
  uint32_t value;
} uint32_pair_t;

typedef struct {
    void *key;
    void *value;
} ptr_pair_t;

#define MAP_GET_EX(_map, _key, _idx) \
({ \
    int idx = _idx, i = 0; \
    typeof(_key) key = _key; \
    for (i = 0; i < (int) ARRLEN(_map); i++) { \
        if (key == _map[i].key) { \
            idx = i; \
        } \
    } \
    idx; \
})

#define MAP_GET(_map, _key) MAP_GET_EX(_map, _key, -1)

uint64_t htonx(uint64_t val);
uint64_t ntohx(uint64_t val);
/* return boolean, 1 is true */
int isprime(unsigned long num);
/* get Greatest Common Divisor(GCD) */
unsigned long get_gcd(unsigned long m, unsigned long n);
/* count the num len, numlen(123) = 3 */
int numlen(unsigned long ul);
/* mod(m - n) for unsigned to avoid out of bounds */
unsigned long umod(unsigned long m, unsigned long n, unsigned mod);
typedef int (*stdcmp)(const void *, const void *);
typedef unsigned long (*stdhash)(const void *);
/* int cmp for sort, m is addr of the elem */
int int_cmp(const int *m, const int *n);
int long_cmp(const long *m, const long *n);
int ulong_cmp(const unsigned long *m, const unsigned long *n);
int int32_cmp(const int32_t *m, const int32_t *n);
int int64_cmp(const int64_t *m, const int64_t *n);
int uint8_cmp(const uint8_t *m, const uint8_t *n);
int uint16_cmp(const uint16_t *m, const uint16_t *n);
int uint32_cmp(const uint32_t *m, const uint32_t *n);
int uint64_cmp(const uint64_t *m, const uint64_t *n);
unsigned long int_hash(const int *val);
unsigned long uint8_hash(const uint8_t *val);
unsigned long uint16_hash(const uint16_t *val);
unsigned long uint32_hash(const uint32_t *val);
unsigned long uint64_hash(const uint64_t *val);
unsigned long str_hash(const char *val);
/* conv num to str and return str
 * it's well used in strcpy(dst, num_to_str(str, sizeof(str), 3)
 * so can do it in one line
**/
char *num_to_str(char *str, ssize_t max, size_t num);
char *strncpy_v(char *dst, const char *src, size_t num);
char *strncat_v(char *dst, const char *src, size_t num);
size_t ptr_to_num(const void *buf, size_t len);
/* get the absolute path of relative path */
int get_abs_path(char *dp, int n, const char *rp);
/* get the file size */
off_t filesize(char *filepath);
off_t fsize(int fd);
/* like the time function, but the time is local time */
time_t loctime();
/* support these format:
 * 1.yyyymmddHHMMSS:    20140722134655
 * 2.yyyymmddHHMMSSRRR: 20140722134655100
**/
int time_str_to_type(struct timeval *tv, const char *str);
void msleep(int msec);
/* like the inet_ntoa, but the param is host order */
//char *inet_htoa(struct in_addr in);
int bitget(uint8_t byte, int idx);
int bitset(uint8_t byte, int idx, int val);
/* quick sqrt */
double sqrt_v(const double x);
double sqrt_n_v(const double x, const int n);
/* rand */
int rand_int(int min, int max);
size_t cast_ptr(void *ptr, size_t size, int ntoh);

int IsPrint(const void *p);
int IsSpace(const void *p);
int conv_to_lower(void *dst, size_t dsz, void *src, size_t size);
int conv_to_upper(void *dst, size_t dsz, void *src, size_t size);
int conv_2d_to_5f(void *dst, size_t dsz, void *src, size_t size); // 2d is '-', 5f is '_', for qz
int conv_xxd_str_to_num(void *dst, size_t dsz, void *src, size_t ssz);
// int conv_to_num(void *dst, size_t dsz, void *src, size_t ssz);
int memset_v(int c, size_t n, ...);
int seq_conv(void *dst, size_t dsz, void *src, size_t ssz, int num, int (*conv)(void *, size_t, void *, size_t));
int reverse(void *data, int num, size_t size);
int reverse_v(void *dst, void *src, int num, size_t (*szof)(const void *));
ssize_t bsearchp(const void *key, void *data, size_t num, size_t size, int (*cmp)(const void *, const void *));
/* insert in sorted sequence */
//void *binsert(const void *key, const void *value, void *data, size_t num, size_t size,
//        int (*cmp)(const void *, const void *));
int bremove(const void *key, void *data, size_t num, size_t size,
        int (*cmp)(const void *, const void *));
/* sort var-length array */
void sort_v(void *dst, void *src, int num, size_t (*szof)(const void *), int (*cmp)(const void *, const void *));
void *seq_search(const void *data, size_t num, size_t size, const void *key, int (*cmp)(const void *, const void *));
int seq_insert(void *data, int num, int index, void *ins, int in, size_t size);
int seq_trim_eq(void *data, int n, size_t size, int (*cmp)(const void *, const void *));
int seq_trim_eq_v(void *data, int n, size_t *len, size_t (*szof)(const void *), int (*cmp)(const void *, const void *));
int seq_trim_eq_s(void *data, int num, size_t size, int (*cmp)(const void *, const void *));
int seq_trim_set(void *data, int m, void *set, int n, size_t size, int (*cmp)(const void *, const void *), const int flags);
ssize_t seq_trim_if(void *data, size_t num, size_t size, int (*is)(const void *));
int seq_trim_if_vp(void *data, int num, size_t (*szof)(const void *), int (*cmp)(const void *, const void *), const void *arg);
int seq_merge(void *dst, int dn, void *sa, int an, void *sb, int bn, size_t size, int(*cmp)(const void *, const void *));
void seq_sub_v(void *data, int num, int *nb, int *len, size_t (*szof)(const void *));
ssize_t memcat(void *dst, size_t max, void *pa, size_t al, void *pb, size_t bl);
ssize_t snprintb(char *str, int max, const void *buf, size_t len, const char *format, const char *del);
void printb(const void *buf, int len, const char *format, const char *del);
int snprintfxxd(char *str, size_t strlen, const void *buf, int len);

#endif /* UTIL_H_ */
