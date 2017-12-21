#include <memory>
#include <iostream>

#define CALL(expr) { \
  std::cout << "########### " << #expr << " ###########" << std::endl; \
  expr; \
}

class Dog {
 public:
  Dog() {
    std::cout << "Dog(" << name << ")" << std::endl;
  }

  virtual ~Dog() {
    std::cout << "~Dog(" << name << ")" << std::endl;
  }

  void Speak() {
    std::cout << "Dog(" << name << "): hello" << std::endl;
  }

  std::string name;
};

// dog will be auto delete when func return
void AutoFree() {
  std::unique_ptr<Dog> dog = std::make_unique<Dog>();
  dog->name = "f2";
  dog->Speak();
}

void Move() {
  std::unique_ptr<Dog> dog1 = std::make_unique<Dog>();
  dog1->name = "f2";
  dog1->Speak();

  std::unique_ptr<Dog> dog2 = move(dog1);
  dog2->name = "f3";
  // Segmentation fault
  // dog1->Speak();
  dog2->Speak();
}

int main(int argc, char* argv[]) {
  CALL(AutoFree());
  CALL(Move());
  return 0;
}
