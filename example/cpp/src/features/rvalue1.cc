#include <string>
#include <stdio.h>

class Dog {
 public:
  Dog(int id)
      : id(id) {
    printf("Dog(), id: %d\n", id);
  }

  Dog(const Dog& dog) {
    id = dog.id;
    printf("Dog() copy, id: %d\n", id);
  }

  virtual ~Dog() {
    printf("~Dog(), id: %d\n", id);
  }

  int id;
};

Dog Get(int id) {
  return Dog(id);
}
//
//Dog&& GetRef(int id) {
//  return Dog(id);
//}

void f1() {
  Dog dog = Get(1);
  printf("f1(), dog.id: %d\n", dog.id);
}

void f2() {
  Dog&& dog = Get(2);
  printf("f2(), dog.id: %d\n", dog.id);
}

//void f3() {
//  Dog&& dog = Get(3);
//  printf("f3(), dog.id: %d\n", dog.id);
//}

int main(int argc, char* argv[]) {
  printf("########### f1 ###########\n");
  f1();
  printf("########### f2 ###########\n");
  f2();
//  printf("########### f3 ###########\n");
//  f3();

  return 0;
}
