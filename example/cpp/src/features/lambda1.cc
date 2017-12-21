#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char* argv[]) {
  // Return type deduction
  auto less = [] (auto lhs, auto rhs) { return lhs < rhs; };
  std::cout << std::boolalpha;
  CALL(less(1, 2));       // less(1, 2) = true
  // string compare is len first
  CALL(less("1", "1"));   // less("1", "1") = false
  CALL(less("1", "10"));  // less("1", "10") = true

  return 0;
}
