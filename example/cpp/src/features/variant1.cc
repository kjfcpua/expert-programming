/*
 * variant1.cc
 *
 *  Created on: Sep 30, 2017
 *      Author: hanchen
 */

#include <variant>
#include <iostream>

#define CALL(expr) \
  do { \
    try { \
      expr; \
      std::cout << #expr << " OK" << std::endl; \
    } catch (const std::bad_variant_access& e) { \
      std::cout << #expr << " exception: " << e.what() << std::endl; \
    } \
  } while(0)

int main() {
  std::variant<int, double> v1{11};
  std::variant<int, double> v2;
  std::cout << "v1 = " << std::get<int>(v1) << std::endl;
  std::cout << "v2 = " << std::get<int>(v2) << std::endl;

  CALL(v2 = v1);                    // v1 = 11
  CALL(v2 = std::get<int>(v1));     // v2 = v1
  CALL(v2 = std::get<0>(v1));       // v2 = v1

  CALL(v2 = std::get<double>(v1));  // exception: Unexpected index
  CALL(v2 = std::get<1>(v1));       // exception: Unexpected index

  // no type float
  // CALL(v2 = std::get<float>(v1));

  // index 1 or 1 only
  // CALL(v2 = std::get<3>(v1));

  v1 = 13.1;                       // v1 = (double) 13.1
  std::cout << "v1 = " << std::get<double>(v1) << std::endl;

  CALL(v2 = v1);                    // v2 = v1
  CALL(v2 = std::get<double>(v1));  // v2 = v1
  CALL(v2 = std::get<1>(v1));       // v2 = v1

  return 0;
}
