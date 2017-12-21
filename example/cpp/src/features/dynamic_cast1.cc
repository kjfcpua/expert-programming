#include <iostream>

class Pet {
 public:
  virtual std::string Name() = 0;
};

class Dog : public Pet {
public:
  std::string Name() override {
    // std::cout << "dog" << std::endl;
    return "dog";
  }
};

class Cat : public Pet {
public:
  std::string Name() override {
    // std::cout << "cat" << std::endl;
    return "cat";
  }
};

#define PRINT(var) std::cout << #var << ": " << var << std::endl;

#define SPEAK(pet) \
  if (pet) { \
    std::cout << #pet << ": " << pet->Name() << std::endl; \
  } else { \
    std::cout << #pet << " is null" << std::endl; \
  }

int main(int argc, char* argv[]) {
  Dog* dog1 = new Dog();

  void* p1 = dog1;
  void* p2 = &argc;

  Pet* pet1 = static_cast<Pet*>(dog1);
  Pet* pet2 = static_cast<Pet*>(p1);
  Pet* pet3 = static_cast<Pet*>(p2);

  Pet* pet4 = dynamic_cast<Pet*>(dog1);
  // compile error: 'void' is not a class
  // Pet* pet5 = dynamic_cast<Pet*>(p1);
  // Pet* pet6 = dynamic_cast<Pet*>(p2);

  Cat* cat1 = static_cast<Cat*>(pet1);
  Cat* cat2 = dynamic_cast<Cat*>(pet4);

  PRINT(pet1);
  PRINT(pet2);
  PRINT(pet3);
  PRINT(pet4);

  PRINT(cat1);
  PRINT(cat2);

  SPEAK(pet1);
  SPEAK(pet2);
  // Segmentation fault
  // SPEAK(pet3);
  SPEAK(pet4);

  SPEAK(cat1);
  SPEAK(cat2);

  return 0;
}
