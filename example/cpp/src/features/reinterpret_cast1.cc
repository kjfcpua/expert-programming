#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char* argv[]) {
  int* p1 = new int;
  *p1 = 3;

  // invalid static_cast
  // uintptr_t u1 = static_cast<uintptr_t>(p1);
  uintptr_t u2 = reinterpret_cast<uintptr_t>(p1);

  int* p3 = reinterpret_cast<int*>(u2);

  // cannot convert 'int*' to 'float*'
  // float* p5 = reinterpret_cast<int*>(u2);

  CALL(*p1);
  CALL(u2);
  CALL(*p3);

  return 0;
}
