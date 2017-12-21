# C++11/14/17特性

## 语言强化

### nullptr 

#### 规范
`std::nullptr_t` is the type of the null pointer literal, [`nullptr`](http://en.cppreference.com/w/cpp/language/nullptr).

```c++
typedef decltype(nullptr) nullptr_t;
```

`nullptr`主要是解决`NULL`在重载时候的歧义。

#### 示例
```c++
#include <stdio.h>

void f(int i) {
  printf("i = %d\n", i);
}
void f(char* p) {
  printf("char* %s\n", p);
}

int main(int argc, char* argv[]) {
  // f(NULL);  // error: call to 'f' is ambiguous
  f(nullptr);  // f(char* p): char* (null)

  return 0;
}
```

### constexpr

#### 规范
`const`修饰的常量可以在运行时确定值，而`constexpr`修饰的常量必须在编译期确定值。

#### 示例
```c++
#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char** argv) {
  const int c1 = argc;
  // is not a constant expression
  // constexpr int c2 = argc;
  // constexpr int c3 = c1;
  CALL(c1);
}
```

### auto

### decltype

#### 规范
```c++
decltype(entity)
decltype(expression)
```

#### 示例1
```c++
#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

#define SWAP(a, b) \
  do { \
    decltype(a) x = a; \
    a = b; \
    b = x; \
  } while(0)

int main(int argc, char* argv[]) {
  int a1 = 1, b1 = 2;
  char a2 = 'x', b2 = 'y';

  SWAP(a1, b1);
  SWAP(a2, b2);

  CALL(a1);
  CALL(b1);

  CALL(a2);
  CALL(b2);

  return 0;
}
```

```shell
$ ./decltype1 
a1: 2
b1: 1
a2: y
b2: x
```

#### 示例2
```c++
#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char* argv[]) {
  auto sum = [](auto a, auto b) {
    return a + b;
  };

  decltype(sum) f = sum;

  CALL(f(1, 2));

  return 0;
}
```

```shell
$ ./decltype2
f(1, 2): 3
```

### override

#### 规范
避免因为函数签名不匹配而没有重载。

#### 示例
```c++
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
```

### cast

#### static_cast

近似于C风格的强制转换方式，转换过程是在编译时进行的，没有运行时的类型检查。

##### 基本数据类型转换

```c++
#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[]) {
  double  d   = 2.71828182;

  int     n   = static_cast<int>(d);
  void*   p   = static_cast<void *>(&n);

  // error: static_cast from 'int *' to 'double *' is not allowed
  // double* dp  = static_cast<double *>(&n);

  std::cout << p << std::endl;

  return 0;
}
```

##### 基类和子类的转换

```c++
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

  B *b2 = static_cast<B*>(&d);
  b2->f(1);  // D::f, n: 1

  return 0;
}
```

#### dynamic_cast

主要用于基类和子类指针的转换，但转换是运行时的。

对比`static_cast`的区别：
- 从子类指针(C* c)转换为基类指针(B* b)，会在编译时检查B是否是C的基类，如果不是的话，则会编译出错；
- 从基类指针(B* b)转化为子类指针(C* c)，会在运行时检查B是否是C的实例，如果不是的话，将返回`nullptr`；

```c++
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
```

```shell
$ ./dynamic_cast1 
pet1: 0x7fe7dd701860
pet2: 0x7fe7dd701860
pet3: 0x7ffee18b87e8
pet4: 0x7fe7dd701860
cat1: 0x7fe7dd701860
cat2: 0x0
pet1: dog
pet2: dog
pet4: dog
cat1: dog
cat2 is null
```

#### reinterpret_cast

```c++
reinterpret_cast < new_type > ( expression )	
```

主要用于指针和非指针数据之间的转换，如某个地址`uintptr_t`和指针`*ptr`之间的互相转换，并且这样的转换不能使用`static_cast`。

```c++
#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char* argv[]) {
  int* p1 = new int;
  *p1 = 3;

  // invalid static_cast
  // uintptr_t u1 = static_cast<uintptr_t>(p1);
  uintptr_t u2 = reinterpret_cast<uintptr_t>(p1);

  int* p3 = reinterpret_cast<int*>(u2);

  // cannot convert 'int*' to 'float*'
  // float* p5 = reinterpret_cast<int*>(u2);

  CALL(*p1);
  CALL(u2);
  CALL(*p3);

  return 0;
}
```

```shell
$ ./reinterpret_cast1 
*p1: 3
u2: 140535025123632
*p3: 3
```

#### const_cast

用于去掉变量的`const`属性。

```c++
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
```

### attribute

#### 规范

```c++
[[ attribute-list ]]                               (since C++11)
[[ using attribute-namespace : attribute-list ]]   (since C++17)
```

```c++
// http://en.cppreference.com/w/cpp/language/attributes

int main(int argc, char* argv[]) {
  // error: unused variable 'n'
  // if -Werror=unused-variable or -Wall on
  int n1 __attribute__((__unused__));  // before C++11
  int n2 [[ gnu::unused ]];            // since  C++11 gun attr
  int n3 [[ maybe_unused ]];           // since  C++11 standard
  return 0;
}
```

## lambda

### 规范

```c++
[capture](parameters) mutable throw() -> return-type { statement }
```

| 符号       | 含义                   |
| -------- | -------------------- |
| []       | 不捕获任何变量              |
| [=]      | 传值捕获所有变量             |
| [var]    | 传值捕获var              |
| [&]      | 引用捕获所有变量             |
| [&var]   | 引用捕获var              |
| [this]   | 传值捕获this，也就是引用捕获当前对象 |
| [*this]  | 传值捕获当前对象             |
| [=,&var] | 引用捕获var，其他变量传值捕获     |
| [&,=var] | 传值捕获var，其他变量引用捕获     |

### Return type deduction

```c++
#include <stdio.h>
#include <iostream>

#define CALL(expr) std::cout << #expr << ": " << expr << std::endl

int main(int argc, char* argv[]) {
  auto less = [] (auto lhs, auto rhs) { return lhs < rhs; };
  std::cout << std::boolalpha;
  CALL(less(1, 2));       // less(1, 2) = true
  // string compare is len first
  CALL(less("1", "1"));   // less("1", "1") = false
  CALL(less("1", "10"));  // less("1", "10") = true

  return 0;
}
```

### constexpr(C++17)
```c++
template <typename T>
constexpr auto func(T n) {
  return [n](T m) { return m + n; }
}
```


## function bind

## smart pointer

### unique_ptr

```c++
template<
    class T,
    class Deleter = std::default_delete<T>
> class unique_ptr;

template <
    class T,
    class Deleter
> class unique_ptr<T[], Deleter>;

template<class T, class... Args>
unique_ptr<T> make_unique(Args&&... args);

template<class T>
unique_ptr<T> make_unique(std::size_t size);

template<class T, class... Args>
/* unspecified */ make_unique(Args&&... args) = delete;
```

- `unique_ptr`将在生命周期结束时自动free/delete；
- `unique_ptr`只能被一个变量拥有，move之后原来变量就立刻失去访问权限；

```c++
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
```

```shell
$ ./unique_ptr1 
########### AutoFree() ###########
Dog()
Dog(f2): hello
~Dog(f2)
########### Move() ###########
Dog()
Dog(f2): hello
Dog(f3): hello
~Dog(f3)
```

### shared_ptr

```c++
#include <memory>

template< class T, class... Args >
shared_ptr<T> make_shared(Args&&... args);
```

- `shared_ptr`是引用计数的指针，当引用为0时才被释放；
- 不要让`shared_ptr`循环引用，否则可能会造成内存泄露；
- 不要让多个`shared_ptr`管理同一个裸指针，建议总是使用`make_shared`创建；

#### 示例
```c++
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

 private:
  std::shared_ptr<Dog> dog;
};


Person* f1() {
  auto dog = std::make_shared<Dog>("wangcai");
  Person* p1 = new Person(dog);
  Person* p2 = new Person(dog);
  delete p1;  // dog has reference will not be delete
  return p2;
}

int main(int argc, char* argv[]) {
  std::cout << "main begin" << std::endl;
  auto p = f1();
  delete p;  // delete dog after delete p
  std::cout << "main end" << std::endl;

  return 0;
}
```

```shell
$ ./shared_ptr1 
main begin
Dog(wangcai)
Person()
Person()
~Person()
~Person()
~Dog(wangcai)
main end
```

### weak_ptr

```c++
template<class T> class weak_ptr;

std::shared_ptr<T> lock() const noexcept;
```

- 弱引用计数的指针，与`shared_ptr`配合使用；
- `weak_ptr`使用前需要用`lock()`获取`shared_ptr`，如果结果为空则说明指针已经被释放；

```c++
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
```

```shell
$ ./weak_ptr1 
main begin
Dog(wangcai)
Person()
Person()
feed Dog(wangcai)
~Person()
~Dog(wangcai)
not dog to feed
~Person()
main end
```

## rvalue

基本概念

左值

右值

亡值

## Structured binding(C++17)

### 规范

```c++
attr(optional) cv-auto ref-operator(optional) [ identifier-list ] = expression ;
attr(optional) cv-auto ref-operator(optional) [ identifier-list ] { expression } ;
attr(optional) cv-auto ref-operator(optional) [ identifier-list ] ( expression ) ;
```
### 示例

```c++
#include <iostream>

struct S {
  double result;
  int err;
};

S divide(auto m, auto n) {
  S s = { 0, 0 };
  if (n == 0) {
    s.err = -1;
    return s;
  }
  s.result = m / n;
  return s;
}

// binding to public data members
#define CALL(expr) \
  do { \
    auto [ret, err] = expr; \
    if (err) { \
      std::cout << #expr << " failed, err: " << err << std::endl; \
    } else { \
      std::cout << #expr << " = " << ret << std::endl; \
    } \
  } while(0)

int main(int argc, char* argv[]) {
  CALL(divide(4.0, 3.0));  // divide(4.0, 3.0) = 1.33333
  CALL(divide(4.0, 0));    // divide(4.0, 0) failed, err: -1

  return 0;
}
```

## Library

### std::array
### std::forward_list
### std::unordered_set
### std::unordered_map

hashmap

### std::tuple(C++11)
#### 规范
```c++
template< class... Types >
class tuple;

template<class... Types>
tuple<VTypes...> make_tuple(Types&&... args);

template<class... Types>
constexpr tuple<VTypes...> make_tuple(Types&&... args);
```

#### 示例
```c++
#include <iostream>
#include <tuple>

std::tuple<double, int> divide(auto m, auto n) {
  if (n == 0) {
    return std::make_tuple(0, -1);
  }
  return std::make_tuple(m / n, 0);
}

#define CALL(expr) \
  do { \
    auto result = expr; \
    auto ret = std::get<0>(result); \
    auto err = std::get<1>(result); \
    if (err) { \
      std::cout << #expr << " failed, err: " << err << std::endl; \
    } else { \
      std::cout << #expr << " = " << ret << std::endl; \
    } \
  } while(0)

int main(int argc, char* argv[]) {
  CALL(divide(4.0, 3.0));  // divide(4.0, 3.0) = 1.33333
  CALL(divide(4.0, 0));    // divide(4.0, 0) failed, err: -1

  return 0;
}
```

```shell
$ ./tuple1 
divide(4.0, 3.0) = 1.33333
divide(4.0, 0) failed, err: -1
```

### std::variant(C++17)
#### 规范
```c++
template <class... Types>
class variant;
```
#### 示例
```c++
#include <variant>
#include <iostream>

#define CALL(expr) \
  do { \
    try { \
      expr; \
      std::cout << #expr << " OK" << std::endl; \
    } catch (const std::bad_variant_access& e) { \
      std::cout << #expr << " throw exception: " << e.what() << std::endl; \
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
```

### std::optional(C++17)
#### 规范
```c++
template< class T >
class optional;
```
#### 示例
```c++
#include <optional>
#include <iostream>

#define CALL(expr) \
  do { \
    try { \
      auto v = expr; \
      std::cout << #expr << " = " << v << std::endl; \
    } catch(const std::bad_optional_access& e) { \
      std::cout << #expr << " throw exception: " << e.what() << std::endl; \
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
  CALL(create(true).value_or("empty"));
  CALL(create(false).value_or("empty"));
  CALL(create(false).value());

  return 0;
}
```

## 类型

|        |                                 |      |
| ------ | ------------------------------- | ---- |
| wchar_ | const wchar_t * wsz = L"hello"; |      |
|        |                                 |      |
|        |                                 |      |
|        |                                 |      |
|        |                                 |      |
|        |                                 |      |

```c++
const char*     sz    = "hello";    // 窄字符串
const wchar_t*  wsz   = L"hello";   // 传统宽字符串
const char*     u8sz  = u8"hello";  // C++11新增UTF-8 编码字符串
const char16_t* u16sz = u"hello";   // C++11新增UTF-16 编码字符串
const char32_t* u32sz = U"hello";   // C++11新增UTF-32 编码字符串
```

## static_assert

编译时断言。

### 规范

```c++
static_assert(bool_constexpr, string);  // from C++11
static_assert(bool_constexpr);          // from C++17      
```

### 示例
```c++
struct S {
  int m1;
  int m2;
};

int main(int argc, char* argv[]) {
  static_assert(sizeof(long) >= 8, "64-bit required.");
  static_assert(sizeof(long) >= 8);

  static_assert(sizeof(S) == sizeof(int) + sizeof(int), "unexpected padding in S");

  return 0;
}
```

## C++17

| feature            | example                     | version |
| ------------------ | --------------------------- | ------- |
| constexpr if       | if (int x = 4; x / 2)       | C++17   |
| constexpr lambda   |                             | C++17   |
| fold expression    |                             | C++17   |
| void_t             |                             | C++17   |
| structured binding | auto [a , b] = getvalues(); | C++17   |
| std::variant       | std::variant<int, float> v; | C++17   |
| string_view        |                             | C++17   |
| parallel STL       |                             | C++17   |
| inline variable    |                             | C++17   |

## 测试环境
```shell
Linux localhost.localdomain 4.4.26-201.el7.centos.x86_64 #1 SMP Thu Oct 20 17:49:17 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
gcc version 7.2.1 20170829 (Red Hat 7.2.1-1) (GCC) 
```

## 参考
- [modern-cpp-features](https://github.com/AnthonyCalandra/modern-cpp-features) 
- [C++17 features](https://github.com/tvaneerd/cpp17_in_TTs)
