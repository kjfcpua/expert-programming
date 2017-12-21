#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

// it's should be (const char*)
int slen(char* str) {
  size_t len = 0;
  while (*str++) {
    len++;
  }
  return len;
}

int main(int argc, char* argv[]) {
  char str[] = {'a', 'b', 'c', '\0'};
  const char* s1 = str;
  // invalid conversion from 'const char*' to 'char*'
  // CALL(slen(s1));
  char* s2 = const_cast<char*>(s1);
  CALL(slen(s2));

  return 0;
}
