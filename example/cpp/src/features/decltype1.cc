#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

#define SWAP(a, b) \
  do { \
    decltype(a) x = a; \
    a = b; \
    b = x; \
  } while(0)

int main(int argc, char* argv[]) {
  int a1 = 1, b1 = 2;
  char a2 = 'x', b2 = 'y';

  SWAP(a1, b1);
  SWAP(a2, b2);

  CALL(a1);
  CALL(b1);

  CALL(a2);
  CALL(b2);

  return 0;
}
