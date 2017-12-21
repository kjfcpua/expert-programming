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
  virtual void f(int) const {
    std::cout << "B::f " << std::endl;
  }
};

class D : public B {
 public:
  // expect override but not
  virtual void f(int) {
    std::cout << "D::f" << std::endl;
  }
};

class G : public B {
 public:
  // must be override
  virtual void f(int) const override {
    std::cout << "G::f" << std::endl;
  }
};

int main(int argc, char* argv[]) {
  D* d = new D();
  B* b = reinterpret_cast<B*>(d);
  d->f(1);  // D::f
  b->f(1);  // B::f, not expect

  G* g = new G();
  b = reinterpret_cast<B*>(g);
  g->f(1);  // G::f
  b->f(1);  // G::f

  return 0;
}
