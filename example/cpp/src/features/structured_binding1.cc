#include <iostream>

struct S {
  double result;
  int err;
};

S divide(auto m, auto n) {
  S s = { 0, 0 };
  if (n == 0) {
    s.err = -1;
    return s;
  }
  s.result = m / n;
  return s;
}

// binding to public data members
#define CALL(expr) \
  do { \
    auto [ret, err] = expr; \
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
