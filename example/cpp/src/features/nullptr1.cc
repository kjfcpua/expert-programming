#include <iostream>

void f(int p) {
  std::cout << p << std::endl;
}
void f(char* p) {
  std::cout << p << std::endl;
}

int main(int argc, char* argv[]) {
  // f(NULL);  // error: call to 'f' is ambiguous
  f(nullptr);  // f(char* p): char* (null)

  return 0;
}
