/*===============================================================
 * test.c
 * Created on: 2014-11-27
 *     Author: chenhan
**===============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <libgen.h>
#include <arpa/inet.h>

#if defined(_OPENSSL)
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#endif

#include "gear.h"

#define NV(v) #v, v

typedef struct {
	char name[20];
	int  sex;
	int  age;
	char addr[20];
	char mobile[13];
} user_t;

/*
 * reference:
 * 1.C/C++ tip: How to detect the operating system type using compiler predefined macros
 * http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
 */
void test_macro(int argc, char **argv) {
#if defined(__linux__)
    printf("__linux__\n");
#endif

#if defined(__unix__)
    printf("__unix__\n");
#endif

#if defined(_WIN32)
    printf("_WIN32\n");
#endif

#if defined(_WIN64)
    printf("_WIN64\n");
#endif

#if defined(__CYGWIN__)
    printf("__CYGWIN__\n");
#endif

#if defined(__APPLE__)
    printf("__APPLE__\n");
#endif

#if defined(__MACH__)
    printf("__MACH__\n");
#endif

/*
 * 198808L for POSIX.1-1988
 * 199009L for POSIX.1-1990
 * 199506L for ISO POSIX.1-1996
 * 200112L for ISO POSIX.1-2001
 * 200809L for ISO POSIX.1-2008
 */
#if defined(_POSIX_VERSION)
    printf("_POSIX_VERSION: %ld\n", _POSIX_VERSION);
#endif

#if defined(__GLIBC__)
    printf("__GLIBC__: %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
#endif
}

#define SIGNED(type) (!!((((type) 1) << (sizeof(type) * 8 - 1)) < 0))
#define CHARD(type)  (sizeof(type) == sizeof(char))
#define SHRTD(type)  (sizeof(type) == sizeof(short))
#define INTED(type)  (sizeof(type) == sizeof(int))
#define LONGD(type)  (sizeof(type) == sizeof(long))
#define TYPED(type)  (CHARD(type) ? "char" : (SHRTD(type) ? "short" : (INTED(type) ? "int" : ((LONGD(type) ? "long" : "")))))
#define PRINT_TYPE_TITLE()   printf("|%15s|%6s|%12s|%8s|\n", "type", "size", "sign", "def")
#define PRINT_TYPE_NUM(type) printf("|%15s|%6zd|%12s|%8s|\n", #type, sizeof(type), SIGNED(type) ? "signed" : "unsigned", TYPED(type))
#define PRINT_TYPE_NUM_1(t1) PRINT_TYPE_NUM(t1)
#define PRINT_TYPE_NUM_2(t1, t2) PRINT_TYPE_NUM_1(t1), PRINT_TYPE_NUM_1(t2)
#define PRINT_TYPE_NUM_3(t1, t2, t3) PRINT_TYPE_NUM_2(t1, t2), PRINT_TYPE_NUM_1(t3)
#define PRINT_TYPE_NUM_4(t1, t2, t3, t4) PRINT_TYPE_NUM_2(t1, t2), PRINT_TYPE_NUM_2(t3, t4)
#define PRINT_TYPE_FLT(type) printf("|%15s|%6zd|%12s|%8s|\n", #type, sizeof(type), "", "")
#define PRINT_TYPE_FLT_1(t1) PRINT_TYPE_FLT(t1)
#define PRINT_TYPE_FLT_2(t1, t2) PRINT_TYPE_FLT_1(t1), PRINT_TYPE_FLT_1(t2)

void test_size(int argc, char **argv) {
  PRINT_TYPE_TITLE();
  PRINT_TYPE_NUM_4(short, int, long, long long);
  PRINT_TYPE_FLT_2(float, double);
  PRINT_TYPE_NUM_2(size_t, ssize_t);
  PRINT_TYPE_NUM_4(uint8_t, uint16_t, uint32_t, uint64_t);
#if defined(__APPLE__)
  PRINT_TYPE_NUM_1(pid_t);
#else
  PRINT_TYPE_NUM_2(pid_t, pthread_t);
#endif
  PRINT_TYPE_NUM(time_t);
}

void test_offsetof(int argc, char **argv) {
  user_t user;
  user_t *puser = &user;

  DEBUGS("OFFSETOF(user_t, age) = %zd", OFFSETOF(user_t, age));
  DEBUGS("OFFSETOF(user_t, mobile) = %zd", OFFSETOF(user_t, mobile));

  DEBUGS("OFFSETOF(*puser, age) = %zd", OFFSETOF(*puser, age));
  DEBUGS("OFFSETOF(*puser, mobile) = %zd", OFFSETOF(*puser, mobile));
}

void test_str_clear(int argc, char **argv) {
  char str[10] = "abcd";
  char *strnull = NULL;

  DEBUGS("before: ");
  DEBUGS("str: %s", str);

  STR_CLEAR(str);
  STR_CLEAR(strnull);

  DEBUGS("after: ");
  DEBUGS("str: %s", str);
}

void test_seq_trim_eq_s(int argc, char **argv) {
  int array[] = { 1, 5, 3, 2, 5, 3 };
  int tn = 0, i;

  tn = seq_trim_eq_s(array, ARRLEN(array), sizeof(array[0]), (stdcmp) int_cmp);
  for (i = 0; i < tn; i++) {
    DEBUGS("%d", array[i]);
  }
}

#define ARR_NUM 10000

size_t szof(const void *val) {
  return sizeof(int);
}

void test_sort_v(int argc, char **argv) {
  int arr[ARR_NUM], dst[ARR_NUM];
  uint8_t scs = 0, dcs = 0;
  int i;

  srandom(time(NULL));
  for (i = 0; i < ARRLEN(arr); i++) {
    arr[i] = rand_int(0, ARR_NUM);
    scs += arr[i];
  }

  sort_v(dst, arr, ARRLEN(arr), szof, (stdcmp) int_cmp);
  for (i = 0; i < ARRLEN(arr); i++) {
    dcs += dst[i];
  }

  DEBUGS("src_cs: %d, dst_cs:%d", scs, dcs);
}

void test_sqrt_v(int argc, char **argv) {
  double d = 0.123456789f;
  printf("%f, %f\n", sqrt(d), sqrt_v(d));
}

void test_get_eth(int argc, char **argv) {
  eth_addr_t eths[256];
  char addr[32], broadaddr[32], netmask[32];
  int num = 0, i;

  num = GetEths(eths, ARRLEN(eths), NULL);
  for (i = 0; i < num; i++) {
    strncpy_v(addr, inet_ntoa(*(struct in_addr *) &eths[i].addr), sizeof(addr));
    strncpy_v(broadaddr, inet_ntoa(*(struct in_addr *) &eths[i].broadaddr),
              sizeof(broadaddr));
    strncpy_v(netmask, inet_ntoa(*(struct in_addr *) &eths[i].netmask),
              sizeof(netmask));
    DEBUGS("name: %s, addr: %s, broadaddr: %s, netmask: %s", eths[i].name, addr,
           broadaddr, netmask);
  }
}

void test_get_pids(int argc, char **argv) {
  pid_t pids[256];
  char name[128];
  strncpy_v(name, basename(argv[0]), sizeof(name));
  int num = get_pids_by_name(pids, ARRLEN(pids), name);
  printf("proc_name: %s, proc_num: %d\n", (char *) name, num);
  for (int i = 0; i < num; i++) {
    printf("pid[%d]: %d\n", i, pids[i]);
  }
}

void test_sync(int argc, char **argv) {
  int num = 0;

  /*
   * n++;
   * __sync_fetch_and_add(10, 3) = 10
   * num = 13
   */
  num = 10;
  printf("__sync_fetch_and_add(%d, %d) = %d\n", 10, 3,
         __sync_fetch_and_add(&num, 3));
  printf("num = %d\n", num);

  /*
   * ++n;
   * __sync_and_add_and_fetch(10, 3) = 13
   * num = 13
   */
  num = 10;
  printf("__sync_and_add_and_fetch(%d, %d) = %d\n", 10, 3,
         __sync_add_and_fetch(&num, 3));
  printf("num = %d\n", num);

  /*
   * CAS, match
   * __sync_val_compare_and_swap(10, 10, 2) = 10
   * num = 2
   */
  num = 10;
  printf("__sync_val_compare_and_swap(%d, %d, %d) = %d\n", 10, 10, 2,
         __sync_val_compare_and_swap(&num, 10, 2));
  printf("num = %d\n", num);

  /*
   * CAS, not match
   * __sync_val_compare_and_swap(10, 3, 5) = 10
   * num = 10
   */
  num = 10;
  printf("__sync_val_compare_and_swap(%d, %d, %d) = %d\n", 10, 3, 5,
         __sync_val_compare_and_swap(&num, 1, 2));
  printf("num = %d\n", num);
}

void test_glb(int argc, char **argv) {
  Glb stGlb, *pstGlb = &stGlb;
  char sBuf[1024 * 1024];
  char *aHost[] = { "192.168.1.1", "192.168.1.2", "192.168.1.3", "192.168.1.4" };
  struct sockaddr_in astRole[16];
  struct sockaddr_in sa;
  int i;

  RegSignal();

  GlbInit(pstGlb, sBuf, 1, ARRLEN(aHost), GLB_ALG_SUCC_RATE);

  printf("--------------------init--------------------\n");
  GlbRoleInit(pstGlb, 0);
  for (i = 0; i < ARRLEN(aHost); i++) {
    memset(&astRole[i], 0, sizeof(astRole[i]));
    astRole[i].sin_addr.s_addr = inet_addr(aHost[i]);
    astRole[i].sin_port = i;
    GlbRoleAdd(pstGlb, 0, &astRole[i], 1, 10000);
  }
  GlbRoleUpdate(pstGlb, 0, 0);
  GlbShow(pstGlb);

  printf("--------------------test1--------------------\n");
  for (i = 0; i < 100000; i++) {
    GlbGet(pstGlb, 0, &sa);
    switch (sa.sin_port) {
      case 0:
        GlbPut(pstGlb, 0, &sa);
        break;
      case 1:
        if (i % 9 != 0) {
          GlbPut(pstGlb, 0, &sa);
        }
        break;
      case 2:
        if (i % 7 != 0) {
          GlbPut(pstGlb, 0, &sa);
        }
        break;
    }
  }
  GlbShow(pstGlb);

  printf("--------------------adjust--------------------\n");
  GlbRoleUpdate(pstGlb, 0, 0);
  GlbShow(pstGlb);

  printf("--------------------test2--------------------\n");
  for (i = 0; i < 100000; i++) {
    GlbGet(pstGlb, 0, &sa);
    if ((i % 2 == 0) || (i % 3 == 0)) {
      GlbPut(pstGlb, 0, &sa);
    }
  }
  GlbShow(pstGlb);

  printf("--------------------adjust--------------------\n");
  GlbRoleUpdate(pstGlb, 0, 0);
  GlbShow(pstGlb);
}

void test_vbuf() {
  char buf[20] = "0123456789";
  VarBuf *vbuf;
  ssize_t blen;

  VarBufCreate(&vbuf, 11);
  VarBufAddBuf(vbuf, buf, 7);
  VarBufAddBuf(vbuf, buf, 9);
  DEBUGS("length: %zd", VarBufLength(vbuf));

  memset(buf, 0, sizeof(buf));
  blen = VarBufGetBuf(vbuf, buf, 13, 0);
  buf[blen] = '\0';
  DEBUGS("get %zd, buf: %s", blen, buf);

  memset(buf, 0, sizeof(buf));
  blen = VarBufGetBuf(vbuf, buf, 13, 0);
  buf[blen] = '\0';
  DEBUGS("get %zd, buf: %s", blen, buf);

  DEBUGS("length: %zd", VarBufLength(vbuf));
}

void test_crypt(int argc, char **argv) {
  unsigned long seed[2];
  char salt[] = "$1$........";
  const char * const seedchars =
      "./0123456789ABCDEFGHIJKLMNOPQRST"
      "UVWXYZabcdefghijklmnopqrstuvwxyz";
  char *password;
  int i;

  /* Generate a (not very) random seed.
   * You should do it better than this... */
  seed[0] = time(NULL);
  seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

  /* Turn it into printable characters from `seedchars'. */
  for (i = 0; i < 8; i++)
    salt[3 + i] = seedchars[(seed[i / 5] >> (i % 5) * 6) & 0x3f];

  /* Read in the user's password and encrypt it. */
  password = crypt(getpass("Password:"), salt);

  /* Print the results. */
  puts(password);
}

void test_machin_like_formula(int argc, char **argv) {
  DEBUGS("TODO");
}

#if defined(_OPENSSL)

static int pem_password_cb_s(char *buf, int size, int rwflag, void *userdata) {
	/* We'd probably do something else if 'rwflag' is 1 */
//	printf("Enter pass phrase for \"%s\"\n", (char *) userdata);
	if (userdata == NULL) {
		return 0;
	}

	char *pass = (char *) userdata;
	int len = strlen(pass);
	if (len <= 0) {
		return 0;
	}

	if (len > size) {
		len = size;
	}

	// memcpy(buf, pass, len);
	memcpy(buf, pass, len);
	buf[len] = '\0';

	printf("%d, %d, %s\n", rwflag, len, buf);

	return len;
}

void test_check_rsa_private_key(int argc, char **argv)
{
	RSA *rsa = NULL;

	// check usage
	if (argc < 2) {
		fprintf(stderr, "%s rsa_key_file_path\n", argv[0]);
		exit(-1);
	}

	OpenSSL_add_all_algorithms();

	// open the RSA public key PEM file
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Open RSA key file %s fail\n", argv[1]);
		exit(-1);
	}

	if (argc < 3) {
		rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
	} else {
		rsa = PEM_read_RSAPrivateKey(fp, NULL, pem_password_cb_s, argv[2]);
	}

	fclose(fp);

	if (rsa == NULL) {
		fprintf(stderr, "Read RSA key file fail\n");
		exit(-1);
	}

	printf("Content of RSA key file:\n");
	printf("##################### BEGIN #####################\n");
	RSA_print_fp(stdout, rsa, 0);
	printf("##################### END #####################\n");
}

void test_check_rsa_public_key(int argc, char **argv)
{
	RSA *rsa = NULL;

	// check usage
	if (argc < 2) {
		fprintf(stderr, "%s rsa_key_file_path\n", argv[0]);
		exit(-1);
	}

	OpenSSL_add_all_algorithms();

	// open the RSA public key PEM file
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Open RSA key file %s fail\n", argv[1]);
		exit(-1);
	}

	rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);

	fclose(fp);

	if (rsa == NULL) {
		fprintf(stderr, "Read RSA key file fail\n");
		exit(-1);
	}

	printf("Content of RSA key file:\n");
	printf("##################### BEGIN #####################\n");
	RSA_print_fp(stdout, rsa, 0);
	printf("##################### END #####################\n");
}

void test_check_rsa_key_pair(int argc, char **argv)
{
	RSA *rkey, *pkey = NULL;

	// check usage
	if (argc < 3) {
		fprintf(stderr, "%s rsa_private_key_file_path rsa_public_key_file_path\n", argv[0]);
		exit(-1);
	}

	OpenSSL_add_all_algorithms();

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Open RSA private key file %s fail\n", argv[1]);
		exit(-1);
	}
	rkey = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
	fclose(fp);

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Open RSA public key file %s fail\n", argv[2]);
		exit(-1);
	}
	pkey = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
	fclose(fp);

	// int ret = EVP_PKEY_cmp(rkey, pkey);
	// printf("Result: %d\n", ret);
}

#endif

int main(int argc, char *argv[]) {
	ptr_pair_t tests[] = {
			{ NV(test_macro) }, { NV(test_size) }, { NV(test_offsetof) },
			{ NV(test_str_clear) },
			{ NV(test_seq_trim_eq_s) }, { NV(test_sort_v) },
			{ NV(test_sqrt_v) },
			{ NV(test_get_eth) }, { NV(test_get_pids) },
			{ NV(test_glb) }, { NV(test_vbuf) }, { NV(test_sync) },
			{ NV(test_crypt) },
			{ NV(test_machin_like_formula) },
#if defined(_OPENSSL)
			{ NV(test_check_rsa_private_key) },
			{ NV(test_check_rsa_public_key) },
#endif
  };

  char *pattern = NULL;
  if (argc > 1) {
    pattern = argv[1];
  }

  char *name = NULL;
  void (*func)(int argc, char *argv[]) = NULL;

  for (int i = 0; i < (int) ARRLEN(tests); i++) {
    name = NULL;
    func = NULL;

    if (pattern == NULL) {
      name = (typeof(name)) tests[i].key;
      DEBUGS("%s", name);
    } else {
      name = (typeof(name)) tests[i].key;
      if (strstr(name, pattern) != NULL) {
        func = (typeof(func)) tests[i].value;
      }
    }

    if (func != NULL) {
      DEBUGS("###################### %s ######################", name);
      func(argc - 1, argv + 1);
    }
  }

  return 0;
}
