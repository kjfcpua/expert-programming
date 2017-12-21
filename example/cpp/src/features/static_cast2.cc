/*
 * nullptr.c
 *
 *  Created on: Sep 30, 2017
 *      Author: hanchen
 */

#include <stdio.h>
#include <iostream>

class B {
 public:
  virtual void f(int n) const {
    std::cout << "B::f, n: " << n << std::endl;
  }
};

class D : public B {
 public:
  virtual void f(int n) const override {
    std::cout << "D::f, n: " << n << std::endl;
  }
};

int main(int argc, char* argv[]) {
  D d;

  B b1 = static_cast<B>(d);
  b1.f(1);   // B::f, n: 1

  B* b2 = static_cast<B*>(&d);
  b2->f(1);  // D::f, n: 1

  return 0;
}
