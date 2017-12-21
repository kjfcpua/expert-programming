/*
 * net_test.c
 *
 *  Created on: Aug 15, 2017
 *      Author: hanchen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define NV(v) #v, v

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ARRLEN(arr) ARRAY_SIZE(arr)

typedef struct {
    void *key;
    void *value;
} ptr_pair_t;

void test_ntoa(int argc, char **argv) {
  struct in_addr sockaddr;
  char ipaddr[20];
  uint32_t s_addr = strtoul(argv[1], NULL, 10);
  sockaddr.s_addr = s_addr;
  strncpy(ipaddr, inet_ntoa(*(struct in_addr *) &sockaddr), sizeof(ipaddr));
  printf("ipaddr: %s, s_addr: %u\n", ipaddr, s_addr);
}

int main(int argc, char *argv[]) {
  ptr_pair_t tests[] = {
      { NV(test_ntoa) },
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
      printf("%s\n", name);
    } else {
      name = (typeof(name)) tests[i].key;
      if (strstr(name, pattern) != NULL) {
        func = (typeof(func)) tests[i].value;
      }
    }

    if (func != NULL) {
      printf("###################### %s ######################\n", name);
      func(argc - 1, argv + 1);
    }
  }

  return 0;
}
