/*
 * nullptr.c
 *
 *  Created on: Sep 30, 2017
 *      Author: hanchen
 */

struct S {
  int m1;
  int m2;
};

int main(int argc, char* argv[]) {
  static_assert(sizeof(long) >= 8, "64-bit required.");
  static_assert(sizeof(long) >= 8);

  static_assert(sizeof(S) == sizeof(int) + sizeof(int), "unexpected padding in S");

  return 0;
}
