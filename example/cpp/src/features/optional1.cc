#include <optional>
#include <iostream>

#define CALL(expr) \
  do { \
    try { \
      auto v = expr; \
      std::cout << #expr << " = " << v << std::endl; \
    } catch(const std::bad_optional_access& e) { \
      std::cout << #expr << " exception: " << e.what() << std::endl; \
  } \
  } while(0)

std::optional<std::string> create(bool b) {
  if (b) {
    return "test";
  } else {
    return {};
  }
}

int main() {
  CALL(create(true).value_or("empty"));   // test
  CALL(create(false).value_or("empty"));  // empty
  CALL(create(false).value());            // exception: bad optional access

  return 0;
}
