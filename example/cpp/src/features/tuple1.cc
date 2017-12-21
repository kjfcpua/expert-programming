#include <iostream>
#include <tuple>

std::tuple<double, int> divide(auto m, auto n) {
  if (n == 0) {
    return std::make_tuple(0, -1);
  }
  return std::make_tuple(m / n, 0);
}

#define CALL(expr) \
  do { \
    auto result = expr; \
    auto ret = std::get<0>(result); \
    auto err = std::get<1>(result); \
    if (err) { \
      std::cout << #expr << " failed, err: " << err << std::endl; \
    } else { \
      std::cout << #expr << " = " << ret << std::endl; \
    } \
  } while(0)

int main(int argc, char* argv[]) {
  CALL(divide(4.0, 3.0));  // divide(4.0, 3.0) = 1.33333
  CALL(divide(4.0, 0));    // divide(4.0, 0) failed, err: -1

  return 0;
}
