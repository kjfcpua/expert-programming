#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char** argv) {
  const int c1 = argc;
  // is not a constant expression
  // constexpr int c2 = argc;
  // constexpr int c3 = c1;
  CALL(c1);
}
