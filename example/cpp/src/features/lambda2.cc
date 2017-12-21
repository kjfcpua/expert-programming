#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

template<typename T>
constexpr auto func(T n) {
  return [n](T m) {
    return m * n;
  };
}

int main(int argc, char* argv[]) {
  auto f1 = func(7);
  CALL(f1(11));

  return 0;
}
