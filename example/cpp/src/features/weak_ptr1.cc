#include <memory>
#include <iostream>

#define CALL(expr) { \
  std::cout << "########### " << #expr << " ###########" << std::endl; \
  expr; \
}

class Dog {
 public:
  Dog(std::string name)
      : name(name) {
    std::cout << "Dog(" << name << ")" << std::endl;
  }

  virtual ~Dog() {
    std::cout << "~Dog(" << name << ")" << std::endl;
  }

  std::string name;
};

class Person {
 public:
  Person(std::shared_ptr<Dog> dog)
      : dog(dog) {
    std::cout << "Person()" << std::endl;
  }

  virtual ~Person() {
    std::cout << "~Person()" << std::endl;
  }

  void Feed() {
    auto d = dog.lock();
    if (d) {
      std::cout << "feed Dog(" << d->name << ")" << std::endl;
    } else {
      std::cout << "not dog to feed" << std::endl;
    }
  }

 private:
  std::weak_ptr<Dog> dog;
};


Person* f1() {
  auto dog = std::make_shared<Dog>("wangcai");
  Person* p1 = new Person(dog);
  Person* p2 = new Person(dog);
  p1->Feed();
  delete p1;
  return p2;
}

int main(int argc, char* argv[]) {
  std::cout << "main begin" << std::endl;
  auto p = f1();
  p->Feed();
  delete p;
  std::cout << "main end" << std::endl;

  return 0;
}
