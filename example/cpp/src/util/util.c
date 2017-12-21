/*
 * util.c
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */
#include "util.h"

uint64_t htonx(uint64_t val) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  uint8_t *arr = (uint8_t *) &val;
  SWAP(arr[0], arr[7]);
  SWAP(arr[1], arr[6]);
  SWAP(arr[2], arr[5]);
  SWAP(arr[3], arr[4]);
#endif
  return val;
}

uint64_t ntohx(uint64_t val) {
  return htonx(val);
}

/*
 * 1 is true
 */
int isprime(unsigned long num) {
  unsigned long s, i;

  s = (typeof(s)) sqrt_v(num);
  for (i = 2; i <= s; i++) {
    if (num % i == 0) {
      return 0;
    }
  }

  return 1;
}

/* get Greatest Common Divisor(GCD) */
unsigned long get_gcd(unsigned long m, unsigned long n) {
  if (m == 0) {
    return n;
  }
  if (n == 0) {
    return m;
  }
  if ((m % 2 == 0) && (n % 2 == 0)) {
    return 2 * get_gcd(m >> 1, n >> 1);
  } else if (m % 2 == 0) {
    return get_gcd(m >> 1, n);
  } else if (n % 2 == 0) {
    return get_gcd(m, n >> 1);
  } else {
    return get_gcd(ABS(m - n), MIN(m, n));
  }
  return 1;
}

int numlen(unsigned long ul) {
  int len = 1;

  while ((ul = ul / 10) != 0) {
    len++;
  }

  return len;
}

unsigned long umod(unsigned long m, unsigned long n, unsigned mod) {
  if (m >= n) {
    return m - n;
  } else {
    return (mod - n) + m;
  }
}

inline int int_cmp(const int *m, const int *n) {
  return NP_CMP(int, m, n);
}

inline int long_cmp(const long *m, const long *n) {
  return NP_CMP(long, m, n);
}

inline int ulong_cmp(const unsigned long *m, const unsigned long *n) {
  return NP_CMP(unsigned long, m, n);
}

inline int int64_cmp(const int64_t *m, const int64_t *n) {
  return NP_CMP(int64_t, m, n);
}

inline int uint8_cmp(const uint8_t *m, const uint8_t *n) {
  return NP_CMP(uint8_t, m, n);
}

inline int uint16_cmp(const uint16_t *m, const uint16_t *n) {
  return NP_CMP(uint16_t, m, n);
}

inline int uint32_cmp(const uint32_t *m, const uint32_t *n) {
  return NP_CMP(uint32_t, m, n);
}

inline int int32_cmp(const int32_t *m, const int32_t *n) {
  return NP_CMP(int32_t, m, n);
}

inline int uint64_cmp(const uint64_t *m, const uint64_t *n) {
  return NP_CMP(uint64_t, m, n);
}

inline unsigned long int_hash(const int *val) {
  return NP_HASH(int, val);
}

inline unsigned long uint8_hash(const uint8_t *val) {
  return NP_HASH(int, val);
}

inline unsigned long uint16_hash(const uint16_t *val) {
  return NP_HASH(int, val);
}

inline unsigned long uint32_hash(const uint32_t *val) {
  return NP_HASH(int, val);
}

inline unsigned long uint64_hash(const uint64_t *val) {
  return NP_HASH(int, val);
}

/**
 * BKDR Hash Function which comes from Brian Kernighan and Dennis Ritchie's book "The C Programming Language"
 */
unsigned long str_hash(const char *val) {
  unsigned long seed = 1313;
  unsigned long hash = 0;
  char *p = NULL;

  p = (typeof(p)) val;
  while (*p != 0) {
    hash = hash * seed + (*p++);
  }

  return (hash & 0x7FFFFFFF);
}

char *num_to_str(char *str, ssize_t max, size_t num) {
  if (max < 0) {
    sprintf(str, "%zd", num);
  } else {
    snprintf(str, max, "%zd", num);
  }
  return str;
}

char *strncpy_v(char *dst, const char *src, size_t num) {
  num = num > 0 ? num - 1 : 0;
  dst[num] = '\0';
  return strncpy(dst, src, num);
}

char *strncat_v(char *dst, const char *src, size_t num) {
  num = num > 0 ? num - 1 : 0;
  dst[num] = '\0';
  return strncat(dst, src, num);
}

size_t ptr_to_num(const void *buf, size_t len) {
  BUILD_BUG_ON(sizeof(size_t) < sizeof(uint32_t));
  size_t num = 0;
  if (len == sizeof(uint8_t)) {
    num = *(uint8_t *) buf;
  } else if (len == sizeof(uint16_t)) {
    num = *(uint16_t *) buf;
  } else if (len == sizeof(uint32_t)) {
    num = *(uint32_t *) buf;
  } else {
    num = *(size_t *) buf;
  }
  return num;
}

int get_abs_path(char *dp, int n, const char *rp) {
  char m, *p = NULL;

  if (*rp != '/') {
    p = getcwd((char *) dp, n);  // get current work dir, notice n must > 0, or may be memory leak
    if (p == dp) {
      m = strlen(dp);
      snprintf(p + m, n - m, "/%s", rp);
      return 0;
    }
  } else {
    snprintf(dp, n, "%s", rp);
    return 0;
  }

  return -1;
}

off_t filesize(char *filepath) {
  off_t len = -1;
  int fd = -1;
  if (filepath == NULL) {
    return -1;
  }
  if ((fd = open(filepath, O_RDONLY)) > 0) {
    len = fsize(fd);
    close(fd);
  }
  return len;
}

off_t fsize(int fd) {
  struct stat st;
  if (fstat(fd, &st) != 0) {
    return -1;
  }
  return st.st_size;
}

WEAK_FUNC_1(msleep, _, void, int msec) {
  usleep(msec * 1000);
}

ALIAS_FUNC_1(msleep, _msleep, void, int);

time_t loctime() {
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return tv.tv_sec - tz.tz_minuteswest * 60;
}

int time_str_to_type(struct timeval *tv, const char *str) {
  struct tm tm;
  char *s = NULL;
  size_t len = 0;

  len = strlen(str);
  if (len == 14) {
    s = strptime(str, "%Y%m%d%H%M%S", &tm);
    if (s == NULL) {
      return -1;
    }
  } else {
    return -1;
  }

  tv->tv_sec = mktime(&tm);
  tv->tv_usec = 0;

  return 0;
}

int bitget(uint8_t byte, int idx) {
  uint8_t bit = 0x01;
  if ((idx < 0) || (idx >= 8)) {
    return -1;
  }
  bit = bit << (7 - idx);
  return byte & bit ? 1 : 0;
}

int bitset(uint8_t byte, int idx, int val) {
  uint8_t bit = 0x01;
  if ((idx < 0) || (idx >= 8)) {
    return -1;
  }
  bit = bit << (7 - idx);
  if (val) {
    byte = byte | bit;
  } else {
    byte = byte & (~bit);
  }
  return byte;
}

double sqrt_v(const double x) {
  return sqrt_n_v(x, 2);
}

/*
 * Newton–Raphson method / Babylonian method
 *
 * tested in 32 and 64 bit system
 *
 * when sqrt(x, 2), f[n+1] = (f[n] + x / f[n]) / 2
 * when sqrt(x, 3), f[n+1] = (f[n] + x / (f[n] * f[n])) / 3
 * ...
 * when sqrt(x, r), f[n+1] = (f[n] + x / (f[n] ^ (r-1))) / r;
 *
 * reference:
 * 1. http://en.wikipedia.org/wiki/Newton's_method
 * 2. http://en.wikipedia.org/wiki/Methods_of_computing_square_roots
 *
 * when (x < 0) and (n < 0) and (n % 2 != 0), will return 0,
 * you should to check and avoid it when call this function
 *
 * x real number
 * n radical number
 */
double sqrt_n_v(const double x, const int n) {
  double d = 0, o = 0, f = 0;
  int i = 0, j = 0;

  if (x > 0) {
    d = x / n;  // get close to result
    /* calc times, by many test, 20 is enough
     * but use 32(64) for safe
     */
    for (i = 0; i < (int) (8 * sizeof(size_t)); i++) {
      f = x;
      for (j = 1; j < n; j++) {
        f = f / d;
      }
      o = d;  // set old value for compare
      d = (f + (n - 1) * d) / n;
      if (ABS(d - o) <= DBL_EPSILON) {
        break;
      }
    }
  } else if (x < 0) {
    if (n % 2 == 0) {
      d = 0;
    } else {
      d = -sqrt_n_v(-x, n);
    }
  }

  return d;
}

/* rand with static seed */
int rand_int(int min, int max) {
  static unsigned seed = 0;
  struct timeval tv;

  if (seed == 0) {
    gettimeofday(&tv, NULL);
    seed = (int) tv.tv_sec * tv.tv_usec;
    srand(seed);
  }

  return (int) (min + (max - min) * (rand() / (RAND_MAX + 10.0)));
}

size_t cast_ptr(void *ptr, size_t size, int ntoh) {
  if (ptr == NULL || size <= 0) {
    return -1;
  }

  switch (size) {
    case sizeof(uint8_t):
      return *(uint8_t *) ptr;
    case sizeof(uint16_t):
      return ntoh ? ntohs(*(uint16_t *) ptr) : *(uint16_t *) ptr;
    case sizeof(uint32_t):
      return ntoh ? ntohs(*(uint32_t *) ptr) : *(uint32_t *) ptr;
    case sizeof(uint64_t):
      return ntoh ? ntohs(*(uint64_t *) ptr) : *(uint64_t *) ptr;
  }

  return 0;
}

int IsPrint(const void *p) {
  return !isprint((int) (*(char *) p));
}

int IsSpace(const void *p) {
  return !isspace((int) (*(char *) p));
}

int reverse(void *data, int num, size_t size) {
  void *l, *r;  // left and right
  char t[num];

  l = data;
  r = OFFOF(data, (num - 1) * size);
  while (l < r) {
    memcpy(t, l, size);
    memcpy(l, r, size);
    memcpy(r, t, size);
    l = OFFOF(l, size);
    r = OFFOF(r, -size);
  }

  return 0;
}

static int conv_int(void *dst, size_t dsz, void *src, size_t ssz,
                    int (*conv)(int)) {
  int val = 0;

  switch (ssz) {
    case sizeof(uint8_t):
      val = conv((int) (*(uint8_t *) src));
      break;
    case sizeof(uint16_t):
      val = conv((int) (*(uint8_t *) src));
      break;
    case sizeof(uint32_t):
      val = conv((int) (*(uint32_t *) src));
      break;
    case sizeof(uint64_t):
      val = conv((int) (*(uint64_t *) src));
      break;
    default:
      return -1;
  }

  switch (dsz) {
    case sizeof(uint8_t):
      *(uint8_t *) dst = val;
      break;
    case sizeof(uint16_t):
      *(uint16_t *) dst = val;
      break;
    case sizeof(uint32_t):
      *(uint32_t *) dst = val;
      break;
    case sizeof(uint64_t):
      *(uint64_t *) dst = val;
      break;
    default:
      return -2;
  }

  return 0;
}

int conv_to_lower(void *dst, size_t dsz, void *src, size_t ssz) {
  return conv_int(dst, dsz, src, ssz, tolower);
}

int conv_to_upper(void *dst, size_t dsz, void *src, size_t ssz) {
  return conv_int(dst, dsz, src, ssz, toupper);
}

// if not 2d, keep
static int _2dto5f(int c) {
  return c == 0x2d ? 0x5f : c;
}

int conv_2d_to_5f(void *dst, size_t dsz, void *src, size_t ssz) {
  return conv_int(dst, dsz, src, ssz, _2dto5f);
}

int conv_xxd_str_to_num(void *dst, size_t dsz, void *src, size_t ssz) {
  int val = 0;

  sscanf((char *) src, "%02x", &val);
  switch (dsz) {
    case sizeof(uint8_t):
      *(uint8_t *) dst = val;
      break;
    case sizeof(uint16_t):
      *(uint16_t *) dst = val;
      break;
    case sizeof(uint32_t):
      *(uint32_t *) dst = val;
      break;
    case sizeof(uint64_t):
      *(uint64_t *) dst = val;
      break;
    default:
      return -2;
  }

  return 0;
}

int memset_v(int c, size_t n, ...) {
  va_list ap;
  void *p = NULL;
  int num = 0;

  va_start(ap, n);
  while (1) {
    p = va_arg(ap, void *);
    if (p == NULL) {
      break;
    }
    memset(p, c, n);
    num++;
  }
  va_end(ap);

  return num;
}

int seq_conv(void *dst, size_t dsz, void *src, size_t ssz, int num,
             int (*conv)(void *, size_t, void *, size_t)) {
  int i;

  for (i = 0; i < num; i++) {
    if (conv(dst, dsz, src, ssz) != 0) {
      return -1;
    }
    dst = OFFOF(dst, dsz);
    src = OFFOF(src, ssz);
  }

  return 0;
}

int reverse_v(void *dst, void *src, int num, size_t (*szof)(const void *)) {
  void *l, *r;
  size_t size = 0;

  l = src;
  r = dst;
  while (num-- > 0) {
    size = szof(l);
    r = OFFOF(r, -size);
    memcpy(r, l, size);
    l = OFFOF(l, size);
  }

  return 0;
}

ssize_t bsearchp(const void *key, const void *data, size_t num, size_t size,
                 int (*cmp)(const void *, const void *)) {
  ssize_t l = 0, r = num - 1, m = 0;
  int ret = -1;
  if (data == NULL || cmp == NULL) {
    return -1;
  }
  while (l <= r) {
    m = (l + r) / 2;
    ret = cmp(key, OFFOF(data, m * size));
    if (ret < 0) {
      r = m - 1;
    } else if (ret > 0) {
      l = m + 1;
    } else {
      break;
    }
  }
  return m;
}

int bremove(const void *key, void *data, size_t num, size_t size,
            int (*cmp)(const void *, const void *)) {
  void *p = bsearch(key, data, num, size, cmp);
  if (p == NULL) {
    return -1;
  }
  memmove(p, OFFOF(p, size), num * size - OFFSET(p, data));
  return 0;
}

__attribute__ ((unused))
static int __cmp_ptr_r(const void *p, const void *q, void *arg) {
  int (*cmp)(const void *,
             const void *) = (int (*)(const void *, const void *)) arg;

  return cmp(*(void **) p, *(void **) q);
}

/* sort pointer */
__attribute__ ((unused))
static void __sort_ptr(void **data, int num, size_t size,
                       int (*cmp)(const void *, const void *)) {
  int i, j;

  for (i = 0; i < num; i++) {
    for (j = num - 1; j > i; j--) {
      if (cmp(data[i], data[j]) > 0) {
        SWAP(data[i], data[j]);
      }
    }
  }
}

void sort_v(void *dst, void *src, int num, size_t (*szof)(const void *),
            int (*cmp)(const void *, const void *)) {
  void **fa = NULL;
  size_t size;
  int i;

  fa = (typeof(fa)) malloc(num * sizeof(void *));

  /* mkdir ptr seq for sort */
  for (i = 0; i < num; i++) {
    fa[i] = src;
    src = OFFOF(src, szof(src));
  }

  /*
   * sort ptr seq by __sort_ptr or qsort_r
   *
   * qsort_r is new sort function from glibc-2.8(2007-11-13) same with qsort
   * qsort_r add a argument (void *arg) which will be pass to
   * cmp function for global work like statics
   *
   * in this used to pass the origion compare function to
   * ptr compare function (__cmp_ptr_r)
   *
   * if earlier glibc, will used __sort_ptr
   **/
#if (defined(__GLIBC__)) && (__GLIBC__ >= 2) && (__GLIBC_MINOR__ >= 8) && (_GLIBC_VERS >= 208)
  qsort_r(fa, num, sizeof(fa[0]), __cmp_ptr_r, (void *) cmp);
#else
  __sort_ptr(fa, num, sizeof(fa[0]), cmp);
#endif

  /* copy data to dst by sorted ptr seq */
  for (i = 0; i < num; i++) {
    size = szof(fa[i]);
    memmove(dst, fa[i], size);
    dst = OFFOF(dst, size);
  }

  free(fa);
}

void *seq_search(const void *data, size_t num, size_t size, const void *key,
                 int (*cmp)(const void *, const void *)) {
  void *p = (typeof(p)) data;
  int i = 0;
  for (i = 0; i < num; i++) {
    // DEBUGS("cmp, ret: %d", cmp(key, data));
    if (cmp(key, p) == 0) {
      return p;
    }
    p = OFFOF(p, size);
  }
  return NULL;
}

int seq_insert(void *data, int num, int index, void *ins, int in, size_t size) {
  if (data == NULL || ins == NULL || size <= 0) {
    return -1;
  }
  memmove(OFFOF(data, (index + in) * size), OFFOF(data, index * size),
          (num - index) * size);
  memmove(OFFOF(data, index * size), ins, in * size);
  return 0;
}

int seq_trim_eq(void *data, int num, size_t size,
                int (*cmp)(const void *, const void *)) {
  void *s = data;
  void *p = NULL;
  int count = 0;

  if (data == NULL || cmp == NULL) {
    return 0;
  }
  if (num <= 0) {
    return 0;
  }

  p = OFFOF(s, size);  // jump over the first
  count = 1;  // this first
  while (num-- > 1) {
    if (cmp(s, p) != 0) {  // not equal
      s = OFFOF(s, size);  // write to next
      if (s != p) {
        memmove(s, p, size);
      }
      count++;
    }
    p = OFFOF(p, size);
  }

  return count;
}

int seq_trim_eq_v(void *data, int num, size_t *len,
                  size_t (*szof)(const void *),
                  int (*cmp)(const void *, const void *)) {
  void *s = data;
  void *p = NULL;
  int count = 0;
  size_t ss, ps, sz = 0;

  if (data == NULL || szof == NULL || cmp == NULL) {
    return 0;
  }
  if (num <= 0) {
    return 0;
  }

  sz = szof(s);
  p = OFFOF(s, sz);
  count = 1;  // first elem
  while (num-- > 1) {
    ss = szof(s);
    ps = szof(p);
    if (cmp(s, p) != 0) {
      s = OFFOF(s, ss);  // s add
      if (s != p) {
        memmove(s, p, ps);
      }
      count++;
      sz += ps;
    }
    p = OFFOF(p, ps);  // p add
  }
  if (len != NULL) {
    *len = sz;
  }

  return count;
}

int seq_trim_eq_s(void *data, int num, size_t size,
                  int (*cmp)(const void *, const void *)) {
  void *s = data;  // dst data
  void *p = NULL;  // cursor
  void *k = NULL;  // find elem
  int count = 0, i;

  if (data == NULL || cmp == NULL) {
    return 0;
  }

  if (num <= 0) {
    return 0;
  }

  p = OFFOF(s, size);  // jump over the first
  count = 1;  // this first
  for (i = 1; i < num; i++) {
    k = seq_search(data, count, size, p, cmp);
    if (k == NULL) {  // not find
      s = OFFOF(s, size);  // write to next
      if (s != p) {
        memmove(s, p, size);
      }
      count++;
    }
    p = OFFOF(p, size);
  }

  return count;
}

int seq_trim_set(void *data, int m, void *set, int num, size_t size,
                 int (*cmp)(const void *, const void *), const int flags) {
  void *p = data, *s = data, *q = set;
  int type, rt;

  type = flags & STRIM_MASK;

  while ((OFFSET(p, data) < (int) (m * size))
      && (OFFSET(q, set) < (int) (num * size))) {
    rt = cmp(p, q);
    if (type == STRIM_EQ) {
      if (rt == 0) {
        memmove(s, p, size);
        s = OFFOF(s, size);
        p = OFFOF(p, size);
      } else if (rt < 0) {
        p = OFFOF(p, size);
      } else {
        q = OFFOF(q, size);
      }
    } else if (type == STRIM_NE) {
      if (rt == 0) {
        p = OFFOF(p, size);
      } else if (rt < 0) {
        memmove(s, p, size);
        s = OFFOF(s, size);
        p = OFFOF(p, size);
      } else {
        q = OFFOF(q, size);
      }
    }
  }

  return (int) (OFFSET(s, data) / size);
}

ssize_t seq_trim_if(void *data, size_t num, size_t size,
                    int (*is)(const void *)) {
  void *s = data;  // dst
  void *p = data;  // src

  while (num-- > 0) {
    if (is(p) != 0) {
      if (p != s) {
        memmove(s, p, size);
      }
      s = OFFOF(s, size);
    }
    p = OFFOF(p, size);
  }

  return (ssize_t) (OFFSET(s, data) / size);
}

int seq_merge(void *dst, int dn, void *sa, int an, void *sb, int bn,
              size_t size, int (*cmp)(const void *, const void *)) {
  return -1;
}

void seq_sub_v(void *data, int num, int *nb, int *len,
               size_t (*szof)(const void *)) {
  size_t sz;
  void *p = data;
  int _num = 0, _len = 0;

  while (num-- > 0) {
    sz = szof(p);
    p = OFFOF(p, sz);
    _num++;
    _len += sz;
    if (nb != NULL && _num >= *nb) {
      break;
    }
    if (len != NULL && _len >= *len) {
      break;
    }
  }
  if (nb != NULL) {
    *nb = _num;
  }
  if (len != NULL) {
    *len = _len;
  }
}

ssize_t memcat(void *dst, size_t max, void *pa, size_t al, void *pb, size_t bl) {
  int ra, rb;

  if ((pa == NULL) || (pb == NULL) || (dst == NULL) || (max < al + bl)) {
    return -1;
  }

  if (((ra = ADD_BUF(dst, (int * ) NULL, pa, al)) < 0)
      || ((rb = ADD_BUF(dst, (int * ) NULL, pb, bl)) < 0)) {
    return -1;
  }

  return al + bl;
}

ssize_t snprintb(char *str, int max, const void *buf, size_t len,
                 const char *format, const char *del) {
  char *p = str;
  int i, n;

  *p = '\0';
  for (i = 0; i < len && (n = max - (int) (p - str)) > 0; i++) {
    p += snprintf(p, n, format, ((uint8_t *) buf)[i] & 0xFF);
    if (del != NULL && (n = max - (int) (p - str)) > 0) {
      p += snprintf(p, n, "%s", del);
    }
  }

  return (ssize_t) (p - str);
}

void printb(const void *buf, int len, const char *format, const char *del) {
  char *p = (char *) buf;

  while (len-- > 0) {
    printf(format, *p++ & 0xFF);
    if (del != NULL) {
      printf("%s", del);
    }
  }
}

int snprintfxxd(char *str, size_t strlen, char *buf) {
  size_t slen = 0;
  char *s = (char *) buf;
  int i = 0;

  slen += (size_t) snprintf(str + slen, strlen - slen, "        ");
  for (i = 0; i < 16; i += 2) {
    slen += (size_t) snprintf(str + slen, strlen - slen, "%02x%02x ", s[i],
                              s[i + 1]);
  }
  slen += (size_t) snprintf(str + slen, strlen - slen, " ");
  for (i = 0; i < 16; i++) {
    if (isgraph(s[i])) {
      slen += (size_t) snprintf(str + slen, strlen - slen, "%c", s[i]);
    } else {
      slen += (size_t) snprintf(str + slen, strlen - slen, ".");
    }
  }

  return slen;
}

