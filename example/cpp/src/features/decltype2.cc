#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char* argv[]) {
  auto sum = [](auto a, auto b) {
    return a + b;
  };

  decltype(sum) f = sum;

  CALL(f(1, 2));

  return 0;
}
