#include <iostream>

int main(int argc, char* argv[]) {
  double  d   = 2.71828182;

  int     n   = static_cast<int>(d);
  void*   p   = static_cast<void*>(&n);

  // FIXME: static_cast from 'int *' to 'double *' is not allowed
  // double* dp  = static_cast<double*>(&n);

  std::cout << p << std::endl;

  return 0;
}
