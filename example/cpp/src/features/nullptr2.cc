#include <iostream>

int main(int argc, char* argv[]) {
  char* p1 = nullptr;
  int*  p2 = nullptr;
  // converting to 'bool' from 'std::nullptr_t' requires direct-initialization
  // bool  b  = nullptr;
  // cannot convert 'std::nullptr_t' to 'int' in initialization
  // int   a  = nullptr;

  std::cout << p1 << p2 << std::endl;

  return 0;
}
