# the golang language
## 关键字
### 总表
| keywords    | 作用                         |
| ----------- | -------------------------- |
| break       | 跳出循环                       |
| case        | switch分支                   |
| chan        | Channel，golang中主要用于同步和传递消息 |
| const       | 常量                         |
| continue    | 继续循环                       |
| default     | switch默认分支                 |
| defer       | 延迟到函数返回前调用                 |
| else        | if ... else ...            |
| fallthrough | switch命中后继续执行              |
| for         | 循环                         |
| func        | 定义函数                       |
| go          | 启动一个goroutine              |
| goto        | 跳转到特定lable                 |
| if          | 条件判断                       |
| import      | 引入包                        |
| interface   | 定义接口                       |
| map         | hash表                      |
| package     | 包路径，类似Java的package         |
| range       | 遍历数据                       |
| return      | 函数返回                       |
| select      | 类似switch，主要用于配合IO使用        |
| struct      | 定义结构                       |
| switch      | 分支判断                       |
| type        | 用来定义类型，类似C/C++的typedef     |
| var         | 定义变量                       |

### for
#### 规范
```go
// Like C/C++ and Java for
for init; condition; post { }

// Like C/C++ and Java while
for condition { }

// Like C/C++ and Java for(;;) or while(1)
for { }
```

#### 示例-标准形式
```go
package main

import (
	"fmt"
)

func main() {
	var n int

	fmt.Printf("########### for1 ###########\n")
	for i := 0; i < 3; i++ {
		fmt.Printf("for1, i: %d\n", i)
	}

	fmt.Printf("########### for2 ###########\n")
	n = 3
	for n >= 0 {
		fmt.Printf("for2, n: %d\n", n)
		n--
	}

	fmt.Printf("########### for3 ###########\n")
	n = 3
	for {
		if n == 0 {
			break
		}
		fmt.Printf("for3, n: %d\n", n)
		n--
	}
}
```
```shell
$ ./for1
########### for1 ###########
for1, i: 0
for1, i: 1
for1, i: 2
########### for2 ###########
for2, n: 3
for2, n: 2
for2, n: 1
for2, n: 0
########### for3 ###########
for3, n: 3
for3, n: 2
for3, n: 1
```

#### 示例-遍历命令行参数
```go
package main

import (
	"fmt"
	"os"
)

func main() {
	var s, sep string
	for i := 1; i < len(os.Args); i++ {
		s += sep + os.Args[i]
		sep = ", "
	}
	fmt.Println("args: [" + s + "]")
}
```
```shell
$ ./for1 2 hello
args: [2, hello]
```

### switch
golang的switch相对C/C++和Java有较大区别：

- 每个case后默认有break；
- 如果需要使用类似C/C++和Java的连续判定，可以使用fallthrough；
- 支持case中的条件判定，类似于多if；
- 支持类型选择；

#### 示例-标准模式
与C/C++和Java的switch case相同行为，但在每个case后默认有break，自然显式break也是可以的。

```go
package main

import (
	"fmt"
)

func main() {
	num := 3
	switch num {
	case 1:
		fmt.Println("num is 1")
	case 3:
		fmt.Println("num is 3")
	case 5:
		fmt.Println("num is 5")
	default:
		fmt.Println("num is not deal")
	}
}
```
```shell
$ ./switch1
num is 3
```

#### 示例-fallthrough
在C/C++和Java中，如果希望case继续执行，那么不写break即可，但golang每个case默认带有break，使用fallthrough表示在case命中后继续执行。

```go
package main

import (
	"fmt"
)

func main() {
	num := 3
	switch {
	case 1 >= num:
		fmt.Println("num >= 1")
		fallthrough
	case 3 >= num:
		fmt.Println("num >= 3")
		fallthrough
	case 5 >= num:
		fmt.Println("num >= 5")
	default:
		fmt.Println("num is not deal")
	}
}
```
```shell
$ ./switch4
num >= 3
num >= 5
```

#### 示例-条件判定
每一条case都是一个条件判定，golang将会逐条的判定条件。

```go
package main

import (
	"fmt"
)

func main() {
	num := 3
	switch {
	case 1 >= num:
		fmt.Println("num >= 1")
	case 3 >= num:
		fmt.Println("num >= 3")
	case 5 >= num:
		fmt.Println("num >= 5")
	default:
		fmt.Println("num is not deal")
	}
}
```
```shell
$ ./switch2
num >= 3
```

#### 示例-type
```go
package main

import (
	"fmt"
)

func main() {
	typeof := func(v interface{}) {
		switch v.(type) {
		case bool:
			fmt.Println("bool")
		case int:
			fmt.Println("int")
		case string:
			fmt.Println("string")
		default:
			fmt.Println("unknow")
		}
	}
	typeof(true)
	typeof(0)
	typeof(-1)
	typeof("hello")
	typeof(1.0)
}
```
```shell
$ ./switch3
bool
int
int
string
unknow
```

### return
golang的return与C/C++和Java有较大区别：

- 可以定义命名返回值，然后使用空return返回；
- 可以返回多个值；

#### 示例-命名返回值
如果命名了返回值，那么可以空return，如果没有命名返回值，则不允许空return。

```go
package main

import (
	"fmt"
)

func f1() (result int) {
	result = 3
	return
}

func f2() (result int) {
	result = 3
	return 5
}

func f3() (result int) {
	return
}

func f4() int {
	return 7
}

// not enough arguments to return
// func f5() int {
// 	return
// }

func main() {
	fmt.Println("f1(): ", f1())
	fmt.Println("f2(): ", f2())
	fmt.Println("f3(): ", f3())
	fmt.Println("f4(): ", f4())
}
```
```shell
$ ./return1
f1():  3
f2():  5
f3():  0
f4():  7
```

#### 示例-多个返回值
```go
package main

import (
	"fmt"
)

func divide(dividend, divisor int) (quotient float32, remainder int) {
	quotient = (float32)(dividend) / (float32)(divisor)
	remainder = dividend % divisor
	return
}

func main() {
	datas := []struct {
		dividend int
		divisor  int
	}{
		{4, 3},
		{4, 2},
	}

	for _, data := range datas {
		quotient, remainder := divide(data.dividend, data.divisor)
		fmt.Printf("divide(%d, %d), quotient: %f, remainder: %d\n",
			data.dividend, data.divisor, quotient, remainder)
	}
}
```
```shell
$ ./return2
divide(4, 3), quotient: 1.333333, remainder: 1
divide(4, 2), quotient: 2.000000, remainder: 0
```

### goto
#### 示例
跳转到label，在现代编程设计中都不建议使用。

```go
package main

import (
	"fmt"
)

func main() {
	arr := []int{1, 3, 4, 5}
	for _, num := range arr {
		if num%2 == 0 {
			fmt.Println(num, "is odd")
			goto END
		} else {
			fmt.Println(num, "is even")
		}
	}

END:
	fmt.Println("end")
}
```
```shell
$ ./goto1
1 is even
3 is even
4 is odd
end
```

### range

#### 规范

range用于遍历array、slice、string、map的关键字，官方描述如下：
```
The range form of the for loop iterates over a slice or map.
When ranging over a slice, two values are returned for each iteration. The first is the index, and the second is a copy of the element at that index.
```

- range展开为循环变量时，array、slice和string返回为(index, elem)，map返回为(key, value)；
- 循环变量在循环初始化时分配，之后都是将值拷贝到循环变量中，循环变量的地址保持不变；

#### 示例

```go
package main

import (
	"fmt"
)

func main() {
	array := [...]int{1, 2, 3, 4}
	slice := []int{1, 2, 3, 4, 5}
	map1 := map[int]int{1: 11, 2: 12, 3: 13}

	fmt.Printf("########### array ###########\n")
	for index, elem := range array {
		fmt.Printf("index: %d, elem: %d, &index: %p, &elem: %p\n", index, elem, &index, &elem)
	}

	fmt.Printf("########### slice ###########\n")
	for index, elem := range slice {
		fmt.Printf("index: %d, elem: %d, &index: %p, &elem: %p\n", index, elem, &index, &elem)
	}

	fmt.Printf("########### str ###########\n")
	for index, elem := range "hello" {
		fmt.Printf("index: %d, elem: %c, &index: %p, &elem: %p\n", index, elem, &index, &elem)
	}

	fmt.Printf("########### map1 ###########\n")
	for key, value := range map1 {
		fmt.Printf("key: %d, value: %d, &key: %p, &value: %p\n", key, value, &key, &value)
	}

	fmt.Printf("########### map1 ###########\n")
	for key := range map1 {
		fmt.Printf("key: %d, &key: %p\n", key, &key)
	}
}
```

```shell
$ ./range1 
########### array ###########
index: 0, elem: 1, &index: 0xc420014088, &elem: 0xc420014090
index: 1, elem: 2, &index: 0xc420014088, &elem: 0xc420014090
index: 2, elem: 3, &index: 0xc420014088, &elem: 0xc420014090
index: 3, elem: 4, &index: 0xc420014088, &elem: 0xc420014090
########### slice ###########
index: 0, elem: 1, &index: 0xc4200140d0, &elem: 0xc4200140d8
index: 1, elem: 2, &index: 0xc4200140d0, &elem: 0xc4200140d8
index: 2, elem: 3, &index: 0xc4200140d0, &elem: 0xc4200140d8
index: 3, elem: 4, &index: 0xc4200140d0, &elem: 0xc4200140d8
index: 4, elem: 5, &index: 0xc4200140d0, &elem: 0xc4200140d8
########### str ###########
index: 0, elem: h, &index: 0xc420014128, &elem: 0xc420014130
index: 1, elem: e, &index: 0xc420014128, &elem: 0xc420014130
index: 2, elem: l, &index: 0xc420014128, &elem: 0xc420014130
index: 3, elem: l, &index: 0xc420014128, &elem: 0xc420014130
index: 4, elem: o, &index: 0xc420014128, &elem: 0xc420014130
########### map1 ###########
key: 1, value: 11, &key: 0xc420014178, &value: 0xc420014180
key: 2, value: 12, &key: 0xc420014178, &value: 0xc420014180
key: 3, value: 13, &key: 0xc420014178, &value: 0xc420014180
########### map1 ###########
key: 1, &key: 0xc4200141b8
key: 2, &key: 0xc4200141b8
key: 3, &key: 0xc4200141b8
```

### go

#### 规范

goroutine是golang最重要的特性之一。

- goroutine是并发(Concurrent)而不是并行(Parallel)的；
- goroutine调用的顺序是不确定的；
- goroutine启动后将占用CPU直到结束；

#### 示例

```go
package main

import (
	"fmt"
	"sync"
)

func main() {
	fmt.Println("main begin.")
	var wg sync.WaitGroup
	for i := 0; i < 3; i++ {
		wg.Add(1)
		fmt.Printf("go: %d\n", i)
		go func(n int) {
			fmt.Printf("begin: %d\n", n)
			defer fmt.Printf("done: %d\n", n)
			wg.Done()
		}(i)
	}
	wg.Wait()
	fmt.Println("main exit.")
}
```
```shell
$ ./go1
main begin.
go: 0
go: 1
go: 2
begin: 2
done: 2
begin: 1
done: 1
begin: 0
done: 0
main exit.
```

### type
#### struct
##### 示例
Pet是基类，Dog是子类，Dog继承Pet。

```go
package main

import "fmt"

// Pet is a representation of a pet
type Pet struct {
	name string
	age  int
}

// Dog is a representation of a dog
type Dog struct {
	Pet
	strain string
}

// Show Dog
func (dog *Dog) Show() {
	fmt.Printf("Dog, name: %s, age: %d, strain: %s\n",
		dog.name, dog.age, dog.strain)
}

func main() {
	datas := []struct {
		name   string
		age    int
		strain string
	}{
		{"Wang", 18, "Chinese Rural Dog"},
		{"Lucy", 19, "Labrador"},
		{"Max", 18, "Wolfdog"},
	}

	for _, data := range datas {
		p := new(Dog)
		p.name = data.name
		p.age = data.age
		p.strain = data.strain
		p.Show()
	}
}
```
```shell
$ ./type1
Dog, name: Wang, age: 18, strain: Chinese Rural Dog
Dog, name: Lucy, age: 19, strain: Labrador
Dog, name: Max, age: 18, strain: Wolfdog
```

#### interface
Golang中的interface设计是非侵入式的，如果一个类型实现了interface的所有方法，这个类型就是interface的实现。

##### 示例
Database是基础接口，Mssql和Oracle分别实现了Database的接口，main中实现了运行时多态。

```go
package main

import (
	"fmt"
)

// Database is interface
type Database interface {
	Show()
}

// Mssql is a representation of a mssql
type Mssql struct {
}

// Show for Mssql implement Database
func (database *Mssql) Show() {
	fmt.Printf("Mssql\n")
}

// Oracle is a representation of a oracle
type Oracle struct {
}

// Show for Oracle implement Database
func (database *Oracle) Show() {
	fmt.Printf("Oracle\n")
}

func main() {
	var d1 Database = new(Mssql)
	var d2 Database = new(Oracle)
	d1.Show()
	d2.Show()
}
```
```shell
$ ./type2
Mssql
Oracle
```

### defer
#### 规范
defer部分类似于Java的finally，用于释放资源，但在函数完成后执行。

- defer后的语句将会立刻展开，所有参数值将会被拷贝，如果参数是函数则会立刻计算出结果；
- derfer延迟到函数return执行完之后才执行；
- defer的执行顺序是FILO，越先调用的defer则越后执行；

逻辑上，defer将会修改return的流程，并且可以修改return的返回值。

对于代码：

```go
func f() (result int) {
	defer f1(f3())
	defer f2()
	return xxx
}
```

golang的逻辑如下：

```go
func f() (result int) {
	r3 := f3()
	result = xxx
	f2()
	f1(r3)
	return
}
```

#### 示例1
DoIO()中通过defer Close()，避免每个返回分支都Close()。

```go
package main

import (
	"errors"
	"fmt"
)

type File struct {
	name string
}

func (file *File) Open(name string) error {
	if len(name) == 0 {
		return errors.New("empty file name")
	}
	file.name = name
	fmt.Printf("Open file succ, name: %s\n", file.name)
	return nil
}

func (file *File) Read() string {
	fmt.Printf("Read file succ, name: %s\n", file.name)
	return "abcd1234"
}

func (file *File) Close() {
	fmt.Printf("Close file succ, name: %s\n", file.name)
}

func DoIO(name string) (string, error) {
	defer fmt.Printf("DoIO done, name: %s\n", name)
	file := new(File)
	err := file.Open(name)
	if err != nil {
		fmt.Printf("Open file fail, name: %s\n", name)
		return "", err
	}
	defer file.Close()
	return file.Read(), nil
}

func main() {
	datas := []struct {
		name string
	}{
		{"Readme.txt"},
		{""},
	}

	for _, data := range datas {
		cont, err := DoIO(data.name)
		if err != nil {
			fmt.Printf("DoIO fail, error: %s, name: %s\n", err.Error(), data.name)
		} else {
			fmt.Printf("DoIO succ, cont: %s, name: %s\n", cont, data.name)
		}
	}
}
```

```shell
$ ./defer1
Open file succ, name: Readme.txt
Read file succ, name: Readme.txt
Close file succ, name: Readme.txt
DoIO done, name: Readme.txt
DoIO succ, cont: abcd1234, name: Readme.txt
Open file fail, name:
DoIO done, name:
DoIO fail, error: empty file name, name:
```

#### 示例2
```go
package main

import (
	"fmt"
)

func f1() (r int) {
	defer func() {
		r++
	}()
	return 0
}

func f2() (r int) {
	t := 5
	defer func() {
		t = t + 5
	}()
	return t
}

func f3() (r int) {
	defer func(r int) {
		r = r + 5
	}(r)
	return 1
}

func main() {
	fmt.Printf("f1: %d\n", f1()) // 1
	fmt.Printf("f2: %d\n", f2()) // 5
	fmt.Printf("f3: %d\n", f3()) // 1
}
```
```shell
$ ./defer2
f1: 1
f2: 5
f3: 1
```

## 基础数据类型
golang里面没有隐式转换，不同类型只能强转。

### boolean
#### 规范
| type | scope         | remark |
| ---- | ------------- | ------ |
| bool | [false, true] |        |

### number
#### 规范
| type    | scope                                    | remark          |
| ------- | ---------------------------------------- | --------------- |
| int     | 至少是32bits                                |                 |
| uint    | 至少是32bits                                |                 |
| int8    | [-128 ~ 127]                             |                 |
| uint8   | [0 ~ 255]                                |                 |
| int16   | [-32768 ~ 32767]                         |                 |
| uint16  | [0 ~ 65,535]                             |                 |
| int32   | [-2,147,483,648 ~ 2,147,483,647]         |                 |
| uint32  | [0 ~ 4,294,967,295]                      |                 |
| int64   | [-9,223,372,036,854,775,808 ~ 9,223,372,036,854,775,807] |                 |
| uint64  | [0 ~ 18,446,744,073,709,551,615]         |                 |
| uintptr | 与指针等长的整型                                 | 类似于C中的uintptr_t |

### float
#### 规范
| type    | scope                       | remark |
| ------- | --------------------------- | ------ |
| float32 | [+-1e-45 ~ +-3.4*1e38]      |        |
| float64 | [+- 5 *1e-324 ~ 107 *1e308] |        |

### plural
#### 规范
| type       | scope | remark |
| ---------- | ----- | ------ |
| complex64  |       |        |
| complex128 |       |        |

### character
#### 规范
| type | scope               | remark                                   |
| ---- | ------------------- | ---------------------------------------- |
| byte | [0 ~ 255]           | [type byte = uint8](https://github.com/golang/go/issues/21601) |
| rune | [0 ~ 4,294,967,295] | [type rune = int32](https://github.com/golang/go/issues/21601) |

### string
#### 规范
Go语言的字符串的字节使用UTF-8编码标识Unicode文本。
字符串的表示很简单，用双引号("")或者反引号(``)来创建。两者的区别：双引号之间的转义符会被转义，而反引号之间的字符保持不变。

| type   | scope | remark |
| ------ | ----- | ------ |
| string |       |        |

#### 示例
string与byte之间互转。

```go
package main

import (
	"fmt"
)

func main() {
	s1 := "12345"
	c1 := []byte(s1) // string --> []bytes
	c1[0] = 'c'
	s2 := string(c1) // []bytes --> string
	fmt.Printf("s1: %s, s2: %s\n", s1, s2)
}
```
```shell
$ ./string1
s1: 12345, s2: c2345
```

### error
#### 规范
| type  | scope | remark |
| ----- | ----- | ------ |
| error |       |        |

```go
type error interface {
    Error() string
}
```

#### 示例
通过增加一个error的返回值，用于返回函数中无法处理的错误。

```go
package main

import (
	"errors"
	"fmt"
)

func divide(dividend, divisor int) (int, error) {
	if divisor == 0 {
		return 0, errors.New("divide by zero")
	}
	return dividend / divisor, nil
}

func main() {
	datas := []struct {
		dividend int
		divisor  int
	}{
		{4, 2},
		{4, 1},
		{4, 0},
	}

	for _, data := range datas {
		quotient, err := divide(data.dividend, data.divisor)
		if err != nil {
			fmt.Printf("divide fail, error: %s\n", err.Error())
			return
		}
		fmt.Printf("divide succ, quotient: %d\n", quotient)
	}
}
```
```shell
$ ./error1
divide succ, quotient: 2
divide succ, quotient: 4
divide fail, error: divide by zero
```

## 高级数据类型

### nil
`nil`类似于C/C++的`nullptr`和Java的`null`，官方描述如下：

```
nil is a predeclared identifier representing the zero value for a pointer, channel, func, interface, map, or slice type.
```

- `nil`表示`*`、`chan`、`func`、`interface`、`map`、`slice`的0，且这些类型默认初始化为`nil`；
- `nil`不是关键字，而是一个builtin的变量；

golang的`nil`与C/C++和Java的`null`存在一定的差别，主要在于`nil`是有类型的。
- `nil`是有类型的，不同类型的`nil`不能比较；
- 部分类型相同的`nil`也不能比较，比如`[]int`、`map`；
- 空`interface`的`nil`等于`nil`；
- 非空`interface`的`nil`不等于`nil`；
- 相同`interface`相同实现的`nil`相等；
- 相同`interface`不同实现的`nil`不相等；

#### 规范
```go
var nil Type // Type must be a pointer, channel, func, interface, map, or slice type
```

#### 示例1
- `(*int)`和`(*uint)`的nil类型不同，不能比较；
- `([]int)`的变量可以是`nil`，但只能和`nil`比较，相互之间不能比较；
- 空`interface`的`nil`等于`nil`；

```go
package main

import (
	"fmt"
	"reflect"
)

func main() {
	var p1 *int = nil
	var p2 *int = nil
	var p3 *uint = nil
	fmt.Println("typeof(p1):", reflect.TypeOf(p1))
	fmt.Println("typeof(p2):", reflect.TypeOf(p2))
	fmt.Println("typeof(p3):", reflect.TypeOf(p3))

	fmt.Println(p1 == nil) // true
	fmt.Println(p3 == nil) // true
	fmt.Println(p1 == p2)  // true
	// invalid operation (mismatched types *int and *uint)
	// fmt.Println(p1 == p3)

	var p4 []int = nil
	var p5 []int = nil
	var p6 *[]int = nil
	var p7 *[]int = nil
	fmt.Println("typeof(p4):", reflect.TypeOf(p4))
	fmt.Println("typeof(p5):", reflect.TypeOf(p5))
	fmt.Println("typeof(p6):", reflect.TypeOf(p6))
	fmt.Println("typeof(p7):", reflect.TypeOf(p7))

	fmt.Println(p4 == nil) // true
	fmt.Println(p6 == nil) // true
	// invalid operation (slice can only be compared to nil)
	// fmt.Println(p4 == p5)
	fmt.Println(p6 == p7) // true

	var p8 (interface{}) = nil
	var p9 (interface{}) = nil
	fmt.Println("typeof(p8):", reflect.TypeOf(p8))
	fmt.Println("typeof(p9):", reflect.TypeOf(p9))

	fmt.Println(p8 == nil) // true
	fmt.Println(p9 == nil) // true
	fmt.Println(p8 == p9)  // true
}
```

```shell
$ ./nil1 
typeof(p1): *int
typeof(p2): *int
typeof(p3): *uint
true
true
true
typeof(p4): []int
typeof(p5): []int
typeof(p6): *[]int
typeof(p7): *[]int
true
true
true
typeof(p8): <nil>
typeof(p9): <nil>
true
true
true
```

#### 示例2

- 非空`interface`的`nil`不等于`nil`；
- 相同`interface`相同实现的`nil`相等；
- 相同`interface`不同实现的`nil`不相等；

```go
package main

import "fmt"

type Base interface {
	Dosth()
}

type Impl1 struct{}

func (this *Impl1) Dosth() {
}

func GetImpl1() Base {
	return nil
}

func GetImpl2() Base {
	var impl *Impl1 // init with zero, also be nil
	return impl
}

type Impl4 struct{}

func (this *Impl4) Dosth() {
}

func GetImpl4() Base {
	var impl *Impl4 // init with zero, also be nil
	return impl
}

func main() {
	p1 := GetImpl1()
	p2 := GetImpl2()
	p3 := GetImpl2()
	p4 := GetImpl4()

	fmt.Println(p1 == nil) // true
	fmt.Println(p2 == nil) // false
	fmt.Println(p1 == p2)  // false
	fmt.Println(p2 == p3)  // true
	fmt.Println(p2 == p4)  // false
}
```

```shell
$ ./nil2 
true
false
false
true
false
```

### array

#### 规范

array类似于C/C++和Java的数组，官方描述如下：

```go
ArrayType = [ArrayLength]ElementType{}
```

```
An array is a numbered sequence of elements of a single type, called the element type. The number of elements is called the length and is never negative.
```
```
- Arrays are values. Assigning one array to another copies all the elements.
- In particular, if you pass an array to a function, it will receive a copy of the array, not a pointer to it.
- The size of an array is part of its type. The types [10]int and [20]int are distinct.
```

注意数组必须指定len，`var a1 []int`是slice而不是array。

#### 示例
```go
package main

import (
	"fmt"
	"reflect"
	"unsafe"
)

func main() {
	var a1 [3]int32
	var a2 [3][2]int32
	a3 := [3]int32{1, 2}
	a4 := [...]int32{1, 2, 3, 4}
	a5 := [3]int32{2: 12}
	a6 := [...]int32{3: 13}

	// not an expression
	// a1 := [3]uint32
	// a2 := [3][2]uint32

	// array index out of bounds
	// a3 := [3]uint32{1, 2, 3, 4}
	// a5 := [3]uint32{3: 13}

	fmt.Printf("sizeof(int32): %d\n", unsafe.Sizeof((int32)(0)))

	fmt.Printf("a1, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a1), reflect.TypeOf(a1).Kind(), len(a1), cap(a1), unsafe.Sizeof(a1))
	fmt.Printf("a2, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a2), reflect.TypeOf(a1).Kind(), len(a2), cap(a2), unsafe.Sizeof(a2))
	fmt.Printf("a3, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a3), reflect.TypeOf(a1).Kind(), len(a3), cap(a3), unsafe.Sizeof(a3))
	fmt.Printf("a4, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a4), reflect.TypeOf(a1).Kind(), len(a4), cap(a4), unsafe.Sizeof(a4))
	fmt.Printf("a5, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a5), reflect.TypeOf(a1).Kind(), len(a5), cap(a5), unsafe.Sizeof(a5))
	fmt.Printf("a6, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a6), reflect.TypeOf(a1).Kind(), len(a6), cap(a6), unsafe.Sizeof(a6))
}
```

```shell
$ ./array1 
sizeof(int32): 4
a1, type: [3]int32, kind: array, len: 3, cap: 3, size: 12
a2, type: [3][2]int32, kind: array, len: 3, cap: 3, size: 24
a3, type: [3]int32, kind: array, len: 3, cap: 3, size: 12
a4, type: [4]int32, kind: array, len: 4, cap: 4, size: 16
a5, type: [3]int32, kind: array, len: 3, cap: 3, size: 12
a6, type: [4]int32, kind: array, len: 4, cap: 4, size: 16
```

### slice

#### 规范

```go
SliceType = []ElementType{}
```

#### 示例
```go
package main

import (
	"fmt"
	"reflect"
	"unsafe"
)

func main() {
	var s1 []int32               // len: 0, cap: 0
	s2 := []int32{1, 2, 3, 4, 5} // len: 5, cap: 5
	s3 := []int32{2: 12}         // len: 3, cap: 3

	// [start, end), include start, exclude end
	// len: end - start
	// cap: cap - start
	// slice can also init from array
	s4 := s2[:]   // len: 5, cap: 5
	s5 := s2[1:]  // len: 4, cap: 4
	s6 := s2[:2]  // len: 2, cap: 5
	s7 := s2[2:4] // len: 2, cap: 3

	s8 := new([5]int)[0:3]    // len: 3, cap: 5
	s9 := make([]int32, 3, 5) // len: 3, cap: 5

	fmt.Printf("sizeof(int32): %d\n", unsafe.Sizeof((int32)(0)))

	fmt.Printf("s1, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s1), reflect.TypeOf(s1).Kind(), len(s1), cap(s1), unsafe.Sizeof(s1))
	fmt.Printf("s2, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s2), reflect.TypeOf(s2).Kind(), len(s2), cap(s2), unsafe.Sizeof(s2))
	fmt.Printf("s3, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s3), reflect.TypeOf(s3).Kind(), len(s3), cap(s3), unsafe.Sizeof(s3))

	fmt.Printf("s4, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s4), reflect.TypeOf(s4).Kind(), len(s4), cap(s4), unsafe.Sizeof(s4))
	fmt.Printf("s5, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s5), reflect.TypeOf(s5).Kind(), len(s5), cap(s5), unsafe.Sizeof(s5))
	fmt.Printf("s6, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s6), reflect.TypeOf(s6).Kind(), len(s6), cap(s6), unsafe.Sizeof(s6))
	fmt.Printf("s7, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s7), reflect.TypeOf(s7).Kind(), len(s7), cap(s7), unsafe.Sizeof(s7))

	fmt.Printf("s8, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s8), reflect.TypeOf(s8).Kind(), len(s8), cap(s8), unsafe.Sizeof(s8))
	fmt.Printf("s9, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s9), reflect.TypeOf(s9).Kind(), len(s9), cap(s9), unsafe.Sizeof(s9))
}
```

```shell
$ ./slice1 
sizeof(int32): 4
s1, type: []int32, kind: slice, len: 0, cap: 0, size: 24
s2, type: []int32, kind: slice, len: 5, cap: 5, size: 24
s3, type: []int32, kind: slice, len: 3, cap: 3, size: 24
s4, type: []int32, kind: slice, len: 5, cap: 5, size: 24
s5, type: []int32, kind: slice, len: 4, cap: 4, size: 24
s6, type: []int32, kind: slice, len: 2, cap: 5, size: 24
s7, type: []int32, kind: slice, len: 2, cap: 3, size: 24
s8, type: []int32, kind: slice, len: 3, cap: 5, size: 24
s9, type: []int32, kind: slice, len: 3, cap: 5, size: 24
```

### map

#### 规范
```go
m["route"] = 66         // add or update
value := m["route"]     // get value
value, ok := m["route"] // check and get value
delete(m, "route")      // delete
```

[Go maps in action](https://blog.golang.org/go-maps-in-action)：
```
Go provides a built-in map type that implements a hash table.
```
```
Maps are not safe for concurrent use: it's not defined what happens when you read and write to them simultaneously. If you need to read from and write to a map from concurrently executing goroutines, the accesses must be mediated by some kind of synchronization mechanism. One common way to protect maps is with sync.RWMutex.
```

- map是hash table的实现；
- map不是协程安全的，如果有并发读写需要加锁；

对于内建`map`协程不安全的问题，在1.9之后增加了`sync.map`，查询、存储和删除都是平均常数时间，并且可以并发访问。

#### 示例
```go
package main

import (
	"fmt"
)

func main() {
	map1 := map[int]string{1: "1", 2: "2"}
	map2 := make(map[int]string)
	map2[1] = "a"

	fmt.Printf("########### init ###########\n")
	fmt.Printf("len(map1): %d, len(map2): %d\n", len(map1), len(map2))
	for key, value := range map1 {
		fmt.Printf("map1[%d]: %s\n", key, value)
	}
	for key, value := range map2 {
		fmt.Printf("map1[%d]: %s\n", key, value)
	}

	fmt.Printf("########### modify ###########\n")
	map1[1] = "x"
	fmt.Printf("map1[%d]: %s\n", 1, map1[1])

	fmt.Printf("########### delete ###########\n")
	delete(map1, 1)
	fmt.Printf("map1[%d]: %s\n", 1, map1[1])
}
```
```shell
$ ./map1
########### init ###########
len(map1): 2, len(map2): 1
map1[2]: 2
map1[1]: 1
map1[1]: a
########### modify ###########
map1[1]: x
########### delete ###########
map1[1]:
```

### chan

#### 规范

chan是channel的简写，chan有自己的特殊语法：
```go
ch <- v       // Send v to channel ch.
v := <-ch     // Receive from ch, and assign value to v.
v, ok := <-ch // Receive from ch, and assign ok to false if closed.
```

```
By default, sends and receives block until the other side is ready. This allows goroutines to synchronize without explicit locks or condition variables.
```

- chan的send和receive是阻塞的；
- chan是协程安全的，在不同的goroutine中调用不需要锁来保护；
- chan是引用传递；

#### 示例-信号量

通过chan同步goroutine，保证最多只有coNum个goroutine。

```go
package main

import (
	"fmt"
)

func main() {
	fmt.Println("main begin.")
	coNum := 3
	ch := make(chan int, coNum)
	for i := 0; i < 5; i++ {
		ch <- 1
		fmt.Printf("go: %d\n", i)
		go func(n int) {
			defer func() { <-ch }()
			fmt.Printf("handle: %d\n", n)
		}(i)
	}
	for i := 0; i < coNum; i++ {
		ch <- 1
	}
	fmt.Println("main exit.")
}
```

```shell
$ ./chan1 
main begin.
go: 0
go: 1
go: 2
handle: 2
go: 3
handle: 3
go: 4
handle: 4
handle: 1
handle: 0
main exit.
```

#### 示例-生产消费者模型

- 通过chan在生产者和消费者之间传递数据(ch)和同步状态(done)；
- chan作为参数传递时是引用传递，不需要使用指针；
- chan是协程安全的，多个goroutine之间不需要锁；
- chan的close事件可以被recv获取，close事件一定在正常数据读完之后，机制类似于read到EOF；

```go
package main

import (
	"fmt"
)

func Producer(ch chan int) {
	for i := 1; i <= 10; i++ {
		fmt.Printf("Producer add, value: %d\n", i)
		ch <- i
	}
	close(ch)
}

func Consumer(id int, ch chan int, done chan bool) {
	for {
		value, ok := <-ch
		if ok {
			fmt.Printf("Consumer, id: %d, recv: %d\n", id, value)
		} else {
			fmt.Printf("Consumer, id: %d, closed\n", id)
			break
		}
	}
	done <- true
}

func main() {
	// if umcomment this line, what happend ?
	// runtime.GOMAXPROCS(1)

	ch := make(chan int, 3)

	coNum := 2
	done := make(chan bool, coNum)
	for i := 1; i <= coNum; i++ {
		go Consumer(i, ch, done)
	}

	go Producer(ch)

	for i := 1; i <= coNum; i++ {
		<-done
	}
}
```

如果程序在多(2+)核CPU下运行，Consumer是交替运行的。
```shell
$ ./chan2 
Producer add, value: 1
Producer add, value: 2
Producer add, value: 3
Producer add, value: 4
Producer add, value: 5
Producer add, value: 6
Consumer, id: 2, recv: 2
Consumer, id: 2, recv: 3
Consumer, id: 2, recv: 4
Consumer, id: 1, recv: 1
Consumer, id: 1, recv: 6
Producer add, value: 7
Consumer, id: 2, recv: 5
Producer add, value: 8
Consumer, id: 1, recv: 7
Producer add, value: 9
Producer add, value: 10
Consumer, id: 1, recv: 9
Consumer, id: 1, recv: 10
Consumer, id: 1, closed
Consumer, id: 2, recv: 8
Consumer, id: 2, closed
```

如果是单核CPU或者开启`runtime.GOMAXPROCS(1)`(goroutine数量)，其实只有一个Consumer在运行。
```shell
$ ./chan2 
Producer add, value: 1
Producer add, value: 2
Producer add, value: 3
Producer add, value: 4
Consumer, id: 1, recv: 1
Consumer, id: 1, recv: 2
Consumer, id: 1, recv: 3
Consumer, id: 1, recv: 4
Producer add, value: 5
Producer add, value: 6
Producer add, value: 7
Producer add, value: 8
Producer add, value: 9
Consumer, id: 1, recv: 5
Consumer, id: 1, recv: 6
Consumer, id: 1, recv: 7
Consumer, id: 1, recv: 8
Consumer, id: 1, recv: 9
Producer add, value: 10
Consumer, id: 1, recv: 10
Consumer, id: 1, closed
Consumer, id: 2, closed
```

## 内建函数

### len
- len用于获取array、slice、map、string、chan的length；
- 如果参数是数组指针，则返回数组的length；
- 如果参数是chan，则返回queued(unread)的数量；
- 如果参数是nil，则返回0；

#### 规范
```go
func len(v Type) int
```

#### 示例
```go
package main

import (
	"fmt"
	"reflect"
)

func main() {
	var a1 []int
	var a2 [2]int
	a3 := [...]int{1, 2, 3}
	var a4 *[]int = new([]int) // invalid argument (type *[]int) for len
	var a5 *[5]int = new([5]int)
	a6 := new([6]int)

	fmt.Println("a1, type:", reflect.TypeOf(a1), ", len:", len(a1))
	fmt.Println("a2, type:", reflect.TypeOf(a2), ", len:", len(a2))
	fmt.Println("a3, type:", reflect.TypeOf(a3), ", len:", len(a3))
	fmt.Println("a4, type:", reflect.TypeOf(a4))
	fmt.Println("a5, type:", reflect.TypeOf(a5), ", len:", len(a5))
	fmt.Println("a6, type:", reflect.TypeOf(a6), ", len:", len(a6))
}
```
```shell
$ ./len1
a1, type: []int , len: 0
a2, type: [2]int , len: 2
a3, type: [3]int , len: 3
a4, type: *[]int
a5, type: *[5]int , len: 5
a6, type: *[6]int , len: 6
```

### cap

- cap用于获取array、slice、chan的capacity；
- 如果参数是数组指针，则返回数组的capacity；
- 如果参数是nil，则返回0；

#### 规范
```go
func cap(v Type) int
```

#### 示例
```go
package main

import (
	"fmt"
	"reflect"
)

func main() {
	var a1 []int
	var a2 [2]int
	a3 := [...]int{1, 2, 3}
	var a4 *[]int = new([]int) // invalid argument (type *[]int) for cap
	var a5 *[5]int = new([5]int)
	a6 := new([6]int)

	fmt.Println("a1, type:", reflect.TypeOf(a1), ", cap:", cap(a1))
	fmt.Println("a2, type:", reflect.TypeOf(a2), ", cap:", cap(a2))
	fmt.Println("a3, type:", reflect.TypeOf(a3), ", cap:", cap(a3))
	fmt.Println("a4, type:", reflect.TypeOf(a4))
	fmt.Println("a5, type:", reflect.TypeOf(a5), ", cap:", cap(a5))
	fmt.Println("a6, type:", reflect.TypeOf(a6), ", cap:", cap(a6))
}
```
```shell
$ ./cap1
a1, type: []int , cap: 0
a2, type: [2]int , cap: 2
a3, type: [3]int , cap: 3
a4, type: *[]int
a5, type: *[5]int , cap: 5
a6, type: *[6]int , cap: 6
```

### len和cap的区别

- 对于string，只有len，没有cap；
- 对于chan，len返回queued(unread)的数量，cap返回chan的容量；

#### 示例
```go
package main

import (
	"fmt"
)

func main() {
	chan1 := make(chan int, 3)
	fmt.Printf("len: %d, cap: %d\n", len(chan1), cap(chan1))

	chan1 <- 5
	fmt.Printf("len: %d, cap: %d\n", len(chan1), cap(chan1))
}
```

```shell
$ ./len2 
len: 0, cap: 3
len: 1, cap: 3
```

### new
与C/C++和Java不同，golang的new不是一个关键字，而是一个内建函数。
new(T)会为T分配一块置零的内存空间，并返回地址(*T)。

#### 规范
```go
func new(Type) *Type
```

### make
#### 规范
```go
func make(t Type, size ...IntegerType) Type
```

### make和new的区别
- new适用于任何对象，而make只适用于slice、map和chan；
- new返回的是指针，而make返回的是对象；
- new将内存全部置零，而make会将对象初始化；

#### 示例
```go
package main

import "fmt"

func main() {
	a1 := new([]int)        // *[]int
	a2 := make([]int, 3)    // []int
	a3 := make([]int, 3, 5) // []int

	fmt.Printf("########### a1 ###########\n")
	fmt.Printf("len(*a1): %d, cap(*a1): %d\n", len(*a1), cap(*a1))
	for i, n := range *a1 {
		fmt.Printf("a1[%d]: %d\n", i, n)
	}

	fmt.Printf("########### a2 ###########\n")
	fmt.Printf("len(a2): %d, cap(a2): %d\n", len(a2), cap(a2))
	for i, n := range a2 {
		fmt.Printf("a2[%d]: %d\n", i, n)
	}

	fmt.Printf("########### a3 ###########\n")
	fmt.Printf("len(a3): %d, cap(a3): %d\n", len(a3), cap(a3))
	for i, n := range a3 {
		fmt.Printf("a3[%d]: %d\n", i, n)
	}
}
```
```shell
$ ./make1
########### a1 ###########
len(*a1): 0, cap(*a1): 0
########### a2 ###########
len(a2): 3, cap(a2): 3
a2[0]: 0
a2[1]: 0
a2[2]: 0
########### a3 ###########
len(a3): 3, cap(a3): 5
a3[0]: 0
a3[1]: 0
a3[2]: 0
```

### append
- append只能操作slice；
- 如果cap足够包含新元素，则会直接存储新元素，然后返回当前slice；
- 如果cap不够包含新元素，则会创建一个新slice，cap*=2，拷贝所有元素，然后返回新slice；

#### 规范
```go
func append(slice []Type, elems ...Type) []Type
```

#### 示例
```
package main

import "fmt"

func main() {
	a1 := make([]int, 3, 5)
	fmt.Printf("a1: %p, len: %d, cap: %d\n", a1, len(a1), cap(a1))
	fmt.Println("a1:", a1)

	a2 := append(a1, 4, 5)
	fmt.Printf("a2: %p, len: %d, cap: %d\n", a2, len(a2), cap(a2))
	fmt.Println("a2:", a2)

	a3 := append(a1, 4, 5, 6)
	fmt.Printf("a3: %p, len: %d, cap: %d\n", a3, len(a3), cap(a3))
	fmt.Println("a3:", a3)

	fmt.Printf("a1: %p, len: %d, cap: %d\n", a1, len(a1), cap(a1))
	fmt.Println("a1:", a1)
}
```

```shell
$ ./append1 
a1: 0xc4200180c0, len: 3, cap: 5
a1: [0 0 0]
a2: 0xc4200180c0, len: 5, cap: 5
a2: [0 0 0 4 5]
a3: 0xc42001a0f0, len: 6, cap: 10
a3: [0 0 0 4 5 6]
a1: 0xc4200180c0, len: 3, cap: 5
a1: [0 0 0]
```

### close
#### 规范
```go
func close(c chan<- Type)
```

官方描述如下：

```
The close built-in function closes a channel, which must be either bidirectional or send-only. It should be executed only by the sender, never the receiver, and has the effect of shutting down the channel after the last sent value is received. After the last value has been received from a closed channel c, any receive from c will succeed without blocking, returning the zero value for the channel element.
```

```go
x := <-c     // 0
x, ok := <-c // 0, false
```

- close只能从sender发起；
- close后，receiver不会立刻收到事件，直到所有的数据都被收取，类似于read EOF；
- close后，当所有数据都被收取，chan变为非阻塞的，再次recv，x会返回0，ok会返回false；

### panic
#### 规范

```go
func panic(v interface{})
```

- panic用来停止当前goroutine；
- panic调用后，首先处理defer函数，然后报告错误，错误信息包括panic参数内容；
- panic可以被内建函数recover捕获处理；

#### 示例
```go
package main

import (
	"fmt"
)

func main() {
	fmt.Println("a")
	panic("fault")
	fmt.Println("b")
}
```
```shell
$ ./panic1
a
panic: fault

goroutine 1 [running]:
main.main()
```

### recover

#### 规范
```go
func recover() interface{}
```

- recover用于处理panic状态的goroutine；
- recover的作用是避免panic扩散导致程序退出，panic后函数会返回，panic后的代码不会被执行；
- recover必须在panic前通过defer调用；
- recover不会影响defer的调用，recover之后，剩余的defer仍然会继续执行；

#### 示例
```go
package main

import (
	"fmt"
)

func f() {
	defer fmt.Println("a") // 5
	defer func() {
		fmt.Println("b") // 3
		err := recover()
		if err != nil {
			fmt.Println("error:", err) // 4
		}
	}()
	defer fmt.Println("c") // 2
	fmt.Println("d")       // 1
	panic("fault")
	fmt.Println("e") // never
}

func main() {
	f()
	fmt.Println("f") // 6
}
```
```shell
$ ./recover1 
d
c
b
error: fault
a
f
```

## 高级特性

### 格式化

#### 规范

```go
import "fmt"

func Print(a ...interface{}) (n int, err error)
func Printf(format string, a ...interface{}) (n int, err error)
func Println(a ...interface{}) (n int, err error)

func Sprint(a ...interface{}) string
func Sprintf(format string, a ...interface{}) string
func Sprintln(a ...interface{}) string
```

#### 标识符

##### General
```
%v	the value in a default format
	when printing structs, the plus flag (%+v) adds field names
%#v	a Go-syntax representation of the value
%T	a Go-syntax representation of the type of the value
%%	a literal percent sign; consumes no value
```

The default format for %v is:
```
bool:                    %t
int, int8 etc.:          %d
uint, uint8 etc.:        %d, %#x if printed with %#v
float32, complex64, etc: %g
string:                  %s
chan:                    %p
pointer:                 %p
```

##### Boolean
```
%t	the word true or false
```

##### Integer
```
%b	base 2
%c	the character represented by the corresponding Unicode code point
%d	base 10
%o	base 8
%q	a single-quoted character literal safely escaped with Go syntax.
%x	base 16, with lower-case letters for a-f
%X	base 16, with upper-case letters for A-F
%U	Unicode format: U+1234; same as "U+%04X"
```

##### Float
Floating-point and complex constituents:
```
%b	decimalless scientific notation with exponent a power of two,
	in the manner of strconv.FormatFloat with the 'b' format,
	e.g. -123456p-78
%e	scientific notation, e.g. -1.234456e+78
%E	scientific notation, e.g. -1.234456E+78
%f	decimal point but no exponent, e.g. 123.456
%F	synonym for %f
%g	%e for large exponents, %f otherwise. Precision is discussed below.
%G	%E for large exponents, %F otherwise
```

##### String
String and slice of bytes (treated equivalently with these verbs):
```
%s	the uninterpreted bytes of the string or slice
%q	a double-quoted string safely escaped with Go syntax
%x	base 16, lower-case, two characters per byte
%X	base 16, upper-case, two characters per byte
```

##### Pointer
```
%p	base 16 notation, with leading 0x
```

#### 复合类型

For compound objects, the elements are printed using these rules, recursively, laid out like this:

```
struct:             {field0 field1 ...}
array, slice:       [elem0 elem1 ...]
maps:               map[key1:value1 key2:value2]
pointer to above:   &{}, &[], &map[]
```

#### 控制标识

```
+	always print a sign for numeric values;
	guarantee ASCII-only output for %q (%+q)
-	pad with spaces on the right rather than the left (left-justify the field)
#	alternate format: add leading 0 for octal (%#o), 0x for hex (%#x);
	0X for hex (%#X); suppress 0x for %p (%#p);
	for %q, print a raw (backquoted) string if strconv.CanBackquote
	returns true;
	always print a decimal point for %e, %E, %f, %F, %g and %G;
	do not remove trailing zeros for %g and %G;
	write e.g. U+0078 'x' if the character is printable for %U (%#U).
' '	(space) leave a space for elided sign in numbers (% d);
	put spaces between bytes printing strings or slices in hex (% x, % X)
0	pad with leading zeros rather than spaces;
	for numbers, this moves the padding after the sign
```

### 原子操作

#### 规范

在golang中，基础类型的读写都不是原子的，如果涉及到并发读写，就需要使用原子操作的API，原子操作API主要分为几个类别：

- 读取(Load)；
- 写入(Store)；
- 增减(Add)；
- 交换(Swap)；
- CAS；

```go
import "sync/atomic"

func LoadInt32(addr *int32) (val int32)
func StoreInt32(addr *int32, val int32)
func AddInt32(addr *int32, delta int32) (new int32)
func SwapInt32(addr *int32, new int32) (old int32)
func CompareAndSwapInt32(addr *int32, old, new int32) (swapped bool)
```

### 反射

#### 规范

```go
import "reflect"
```

#### 示例
```go
package main

import (
	"errors"
	"fmt"
	"reflect"
)

type Pig struct {
	name string
}

type Database interface {
	Show()
}

type Oracle struct {
}

func (database *Oracle) Show() {
	fmt.Printf("Oracle\n")
}

func main() {
	var b1 bool = true                   // bool
	var int1 int = 1                     // int
	var int2 int32 = 2                   // int32
	var int3 uint32 = 4                  // uint32
	var int4 uintptr = 7                 // uintptr
	var float1 float32 = 3.14            // float32
	var float2 float64 = 6.18            // float64
	var c1 byte = 'x'                    // uint8
	var c2 rune = 'z'                    // int32
	var str string = "hello"             // string
	var err1 error = nil                 // <nil>
	var err2 error = errors.New("fail")  // *errors.errorString, ptr
	var pig1 Pig                         // main.Pig
	pig2 := new(Pig)                     // *main.Pig, ptr
	var database1 Database               // <nil>
	var database2 Database = new(Oracle) // *main.Oracle, ptr
	ch1 := make(chan int, 3)             // chan int
	var array1 [3]int                    // [3]int, array
	var array2 [3][2]int                 // [3][2]int, array
	var s1 []int                         // []int, slice
	map1 := make(map[int]int)            // map[int]int
	map2 := make(map[int]string)         // map[int]string

	fmt.Printf("b1, type: %s, kind: %s\n", reflect.TypeOf(b1), reflect.TypeOf(b1).Kind())
	fmt.Printf("int1, type: %s, kind: %s\n", reflect.TypeOf(int1), reflect.TypeOf(int1).Kind())
	fmt.Printf("int2, type: %s, kind: %s\n", reflect.TypeOf(int2), reflect.TypeOf(int2).Kind())
	fmt.Printf("int3, type: %s, kind: %s\n", reflect.TypeOf(int3), reflect.TypeOf(int3).Kind())
	fmt.Printf("int4, type: %s, kind: %s\n", reflect.TypeOf(int4), reflect.TypeOf(int4).Kind())
	fmt.Printf("float1, type: %s, kind: %s\n", reflect.TypeOf(float1), reflect.TypeOf(float1).Kind())
	fmt.Printf("float2, type: %s, kind: %s\n", reflect.TypeOf(float2), reflect.TypeOf(float2).Kind())
	fmt.Printf("c1, type: %s, kind: %s\n", reflect.TypeOf(c1), reflect.TypeOf(c1).Kind())
	fmt.Printf("c2, type: %s, kind: %s\n", reflect.TypeOf(c2), reflect.TypeOf(c2).Kind())
	fmt.Printf("str, type: %s, kind: %s\n", reflect.TypeOf(str), reflect.TypeOf(str).Kind())
	fmt.Println("err1: ", reflect.TypeOf(err1))
	fmt.Printf("err2, type: %s, kind: %s\n", reflect.TypeOf(err2), reflect.TypeOf(err2).Kind())
	fmt.Printf("pig1, type: %s, kind: %s\n", reflect.TypeOf(pig1), reflect.TypeOf(pig1).Kind())
	fmt.Printf("pig2, type: %s, kind: %s\n", reflect.TypeOf(pig2), reflect.TypeOf(pig2).Kind())
	fmt.Println("database1: ", reflect.TypeOf(database1))
	fmt.Printf("database2, type: %s, kind: %s\n", reflect.TypeOf(database2), reflect.TypeOf(database2).Kind())
	fmt.Printf("ch1, type: %s, kind: %s\n", reflect.TypeOf(ch1), reflect.TypeOf(ch1).Kind())
	fmt.Printf("array1, type: %s, kind: %s\n", reflect.TypeOf(array1), reflect.TypeOf(array1).Kind())
	fmt.Printf("array2, type: %s, kind: %s\n", reflect.TypeOf(array2), reflect.TypeOf(array2).Kind())
	fmt.Printf("s1, type: %s, kind: %s\n", reflect.TypeOf(s1), reflect.TypeOf(s1).Kind())
	fmt.Printf("map1, type: %s, kind: %s\n", reflect.TypeOf(map1), reflect.TypeOf(map1).Kind())
	fmt.Printf("map2, type: %s, kind: %s\n", reflect.TypeOf(map2), reflect.TypeOf(map2).Kind())
}
```

```shell
$ ./reflect1 
b1, type: bool, kind: bool
int1, type: int, kind: int
int2, type: int32, kind: int32
int3, type: uint32, kind: uint32
int4, type: uintptr, kind: uintptr
float1, type: float32, kind: float32
float2, type: float64, kind: float64
c1, type: uint8, kind: uint8
c2, type: int32, kind: int32
str, type: string, kind: string
err1:  <nil>
err2, type: *errors.errorString, kind: ptr
pig1, type: main.Pig, kind: struct
pig2, type: *main.Pig, kind: ptr
database1:  <nil>
database2, type: *main.Oracle, kind: ptr
ch1, type: chan int, kind: chan
array1, type: [3]int, kind: array
array2, type: [3][2]int, kind: array
s1, type: []int, kind: slice
map1, type: map[int]int, kind: map
map2, type: map[int]string, kind: map
```



### Conditional compiling

#### 规范
条件编译参数是一种特殊注释，必须在源文件第一行，且下面必须再空一行，规范和shell的`#!/bin/sh`类似。
```go
// +build <tags>
```

```shell
go list -f '{{.GoFiles}}'
go list -f '{{.GoFiles}}' --tags exclude
go build -tags <tags>
go test -tags <tags>
```

#### 示例

```go
package main

import (
	"fmt"
)

func init() {
	fmt.Println("main::init()")
}

func main() {
	fmt.Println("main::main()")
}
```

```go
package main

import "fmt"

func init() {
    fmt.Println("inc::init()")
}
```

```go
// +build flag1

package main

import "fmt"

func init(){
    fmt.Println("exc::init()")
}
```

```
$ go list -f '{{.GoFiles}}'
[compile1.go inc1.go]
$ go list -f '{{.GoFiles}}' --tags a
[compile1.go inc1.go]
$ go list -f '{{.GoFiles}}' --tags flag1
[compile1.go exc1.go inc1.go]
$ go build
$ ./compile1
main::init()
inc::init()
main::main()
$ go build -tags flag1
$ ./compile1
main::init()
exc::init()
inc::init()
main::main()
```

### http server

#### 规范
```go
func (f HandlerFunc) ServeHTTP(w ResponseWriter, r *Request)
```
#### 示例
```go
package main

import (
	"fmt"
	"log"
	"net/http"
)

type helloHandler struct {
}

func (handler helloHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "hello, you've hit %s\n", r.URL.Path)
}

func main() {
	err := http.ListenAndServe(":8080", helloHandler{})
	log.Fatal(err)
}
```

运行服务：

```shell
$ ./http1
```

模拟客户端测试：

```shell
$ curl localhost:8080/home
hello, you've hit /home
```

通过浏览器测试：[http://localhost:8080/home](http://localhost:8080/home)

### http client
#### 规范
```go
func (c *Client) Head(url string) (resp *Response, err error)
func (c *Client) Get(url string) (resp *Response, err error)
func (c *Client) Post(url string, contentType string, body io.Reader) (resp *Response, err error)
```
#### 示例
```go
package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	resp, err := http.Get("http://localhost:8080/home")
	if err != nil {
		fmt.Printf("GET fail, error: %s\n", err.Error())
		return
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("read body fail, error: %s\n", err.Error())
		return
	}
	fmt.Printf("########### content ###########\n")
	fmt.Printf("%s", body)
	fmt.Printf("###########   end   ###########\n")
}
```
```
$ ./http2 
########### content ###########
hello, you've hit /home
###########   end   ###########
```

### RPC server
#### 规范
```go
import "net/rpc"

func (t *T) MethodName(argType T1, replyType *T2) error
```

- the method's type is exported.
- the method is exported.
- the method has two arguments, both exported (or builtin) types.
- the method's second argument is a pointer.
- the method has return type error.

#### 示例
```go
package main

import (
	"errors"
	"fmt"
	"net"
	"net/http"
	"net/rpc"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

type Arith int

func (t *Arith) Multiply(args *Args, reply *int) error {
	fmt.Printf("Multiply, a: %d, b: %d\n", args.A, args.B)
	*reply = args.A * args.B
	return nil
}

func (t *Arith) Divide(args *Args, quo *Quotient) error {
	fmt.Printf("Divide, a: %d, b: %d\n", args.A, args.B)
	if args.B == 0 {
		return errors.New("divide by zero")
	}
	quo.Quo = args.A / args.B
	quo.Rem = args.A % args.B
	return nil
}

func main() {
	arith := new(Arith)
	rpc.Register(arith)
	rpc.HandleHTTP()
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		fmt.Printf("listen fail, error: %s\n", err.Error())
		return
	}
	http.Serve(listener, nil)
}
```

通过curl命令，确定RPC服务已经运行了。
```shell
$ curl localhost:8080
404 page not found
```

### RPC client
#### 规范
```go
func Dial(network, address string) (*Client, error)
func DialHTTP(network, address string) (*Client, error)
func (client *Client) Close() error
func (client *Client) Call(serviceMethod string, args interface{}, reply interface{}) error
func (client *Client) Go(serviceMethod string, args interface{}, reply interface{}, done chan *Call) *Call
```

#### 示例1-同步调用
```go
package main

import (
	"fmt"
	"net/rpc"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

type Arith int

func main() {
	client, err := rpc.DialHTTP("tcp", "localhost:8080")
	if err != nil {
		fmt.Printf("dail fail, error: %s\n", err.Error())
		return
	}
	defer client.Close()

	for i := 0; i < 10; i++ {
		// Synchronous call
		args := &Args{i, i + 1}
		var reply int
		err = client.Call("Arith.Multiply", args, &reply)
		if err != nil {
			fmt.Printf("RPC fail, error: %s\n", err.Error())
			return
		}
		fmt.Printf("Arith.Multiply: %d * %d = %d\n", args.A, args.B, reply)
	}
}
```

```shell
$ ./rpc2 
Arith.Multiply: 0 * 1 = 0
Arith.Multiply: 1 * 2 = 2
Arith.Multiply: 2 * 3 = 6
Arith.Multiply: 3 * 4 = 12
Arith.Multiply: 4 * 5 = 20
Arith.Multiply: 5 * 6 = 30
Arith.Multiply: 6 * 7 = 42
Arith.Multiply: 7 * 8 = 56
Arith.Multiply: 8 * 9 = 72
Arith.Multiply: 9 * 10 = 90
```

#### 示例2-异步调用
```go
package main

import (
	"fmt"
	"net/rpc"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

type Arith int

func main() {
	client, err := rpc.DialHTTP("tcp", "localhost:8080")
	if err != nil {
		fmt.Printf("dail fail, error: %s\n", err.Error())
		return
	}
	defer client.Close()

	coNum := 5
	ch := make(chan int, coNum)

	for i := 0; i < 10; i++ {
		ch <- 1
		// Asynchronous call
		args := &Args{100, i}
		quotient := new(Quotient)
		call := client.Go("Arith.Divide", args, quotient, nil)
		go func() {
			defer func() { <-ch }()
			done := <-call.Done
			err = done.Error
			if err != nil {
				fmt.Printf("dail fail, error: %s\n", err.Error())
				return
			}
			fmt.Printf("Arith.Divide: %d / %d = %d, %d\n", args.A, args.B, quotient.Quo, quotient.Rem)
		}()
	}

	for i := 0; i < coNum; i++ {
		ch <- 1
	}
}
```

```shell
$ ./rpc3 
Arith.Divide: 100 / 1 = 100, 0
Arith.Divide: 100 / 2 = 50, 0
Arith.Divide: 100 / 4 = 25, 0
dail fail, error: divide by zero
Arith.Divide: 100 / 3 = 33, 1
Arith.Divide: 100 / 5 = 20, 0
Arith.Divide: 100 / 9 = 11, 1
Arith.Divide: 100 / 7 = 14, 2
Arith.Divide: 100 / 8 = 12, 4
Arith.Divide: 100 / 6 = 16, 4
```

### gRPC server

#### 简介

- gRPC是Google开源的一款高性能RPC框架；
- gRPC面向移动和HTTP/2设计，有助于节省带宽、降低的TCP链接次数、节省CPU使用、和电池寿命；
- gRPC使用Protocol Buffers作为IDL和底层的序列化工具；
- gRPC提供C/C++、Java和Golang的实现，支持跨操作系统、跨编程语言的RPC调用；

#### 安装
在GOPATH下运行如下命令安装。
```shell
go get -u google.golang.org/grpc
```

#### 规范
```go
import "google.golang.org/grpc"

func NewServer(opt ...ServerOption) *Server
func (s *Server) RegisterService(sd *ServiceDesc, ss interface{})
func (s *Server) Serve(lis net.Listener) error
func (s *Server) ServeHTTP(w http.ResponseWriter, r *http.Request)
func (s *Server) Stop()
```

#### 示例

```protobuf
// need use plugins=grpc
// protoc -I . --go_out=plugins=grpc:. *.proto

syntax = "proto3";

package main;

message MultiplyRequest {
	uint32 x = 1;
	uint32 y = 2;
}

message MultiplyResponse {
	uint32 result = 1;
}

service Arith {
	rpc Multiply (MultiplyRequest) returns (MultiplyResponse);
}
```

```go
package main

import (
	"fmt"
	"net"

	proto "../proto"

	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

type server struct{}

func (s *server) Multiply(ctx context.Context, request *proto.MultiplyRequest) (response *proto.MultiplyResponse, err error) {
	// fmt.Printf("gRPC Multiply, request.X: %d, request.Y: %d\n", request.X, request.Y)
	response = &proto.MultiplyResponse{Result: request.X * request.Y}
	return
}

func main() {
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		fmt.Printf("listen fail, error: %s\n", err.Error())
		return
	}
	gsvr := grpc.NewServer()
	defer gsvr.Stop()
	proto.RegisterArithServer(gsvr, &server{})
	gsvr.Serve(listener)
}
```

通过curl命令，确定RPC服务已经运行了。

```shell
$ curl localhost:8080
```

### gRPC client

#### 规范
```go
func Dial(target string, opts ...DialOption) (*ClientConn, error)
func (cc *ClientConn) Close() error
```

#### 示例
```go
package main

import (
	"fmt"

	proto "../proto"

	"golang.org/x/net/context"

	"google.golang.org/grpc"
)

func main() {
	conn, err := grpc.Dial("localhost:8080", grpc.WithInsecure())
	if err != nil {
		fmt.Printf("dail fail, error: %s\n", err.Error())
		return
	}
	defer conn.Close()

	client := proto.NewArithClient(conn)

	for i := 0; i < 10; i++ {
		request := &proto.MultiplyRequest{
			X: uint32(i),
			Y: uint32(i + 1),
		}
		response, err := client.Multiply(context.Background(), request)
		if err != nil {
			fmt.Printf("RPC fail, error: %s\n", err.Error())
			return
		}
		fmt.Printf("Arith.Multiply: %d * %d = %d\n", request.X, request.Y, response.Result)
	}
}
```

```shell
$ ./grpc2 
Arith.Multiply: 0 * 1 = 0
Arith.Multiply: 1 * 2 = 2
Arith.Multiply: 2 * 3 = 6
Arith.Multiply: 3 * 4 = 12
Arith.Multiply: 4 * 5 = 20
Arith.Multiply: 5 * 6 = 30
Arith.Multiply: 6 * 7 = 42
Arith.Multiply: 7 * 8 = 56
Arith.Multiply: 8 * 9 = 72
Arith.Multiply: 9 * 10 = 90
```

## 深入理解

### 协程

#### 协程模型

协程的设计分为两种，一是单线程无锁协程，也就是1:N模型，二是多线程协程，也就是M:N模型。

单线程无锁协程的设计思想是避免使用锁，是传统多进程状态机模型的演化，所有协程都在一个线程中运行，对于多核CPU，需要启动多个进程或线程监听不同的端口或事件，主要代表为C/C++的协程库，例如[云风-C协程库](https://github.com/cloudwu/coroutine/), [Tencent libco](https://github.com/Tencent/libco)等。

多线程协程的设计思想是尽量利用多核，是传统多线程模型的演化，当多协程同步数据时，也避免使用锁，而是通过管道或IPC来通信，代表为goroutine，但部分C/C++的协程库也是这种思路的实现，例如[Baidu bthread](https://github.com/brpc/brpc/blob/master/docs/cn/bthread.md)，[libgo](https://github.com/yyzybb537/libgo)。

#### 协程与线程
| keywords    | 线程                         | 协程                         |
| ----------- | --------------------------- | ---------------------------    |
| 任务调度     | 抢占式，不受应用程序控制        | 非抢占式，应用程序可以主动切换     |  
| 调度级别     | 内核                         | 用户态，换速度要远快于内核调度    |
| 资源占用     | MB级别                       | KB级别                        |              

#### 协程的切换
goroutine协程切换的时机：
- 调用协程切换接口`runtime.Gosched()`；
- `chan`收发阻塞时；
- syscall阻塞时；
- 程序GC；

#### 规范
golang中，协程相关的api都在[runtime](https://golang.org/pkg/runtime)包中。

```go
import "runtime"

// NumCPU returns the number of logical CPUs usable by the current process.
func NumCPU() int

// GOMAXPROCS sets the maximum number of CPUs that can be executing 
// simultaneously and returns the previous setting.
func GOMAXPROCS(n int) int

// NumGoroutine returns the number of goroutines that currently exist.
func NumGoroutine() int

// Gosched yields the processor, allowing other goroutines to run.
func Gosched()
```

在go 1.5之后，程序启动时默认执行了`GOMAXPROCS(NumCPU())`。

#### 数据同步
golang社区流传着一句经典：
```
不要通过共享内存来通信，而应该通过通信来共享内存。
```
[原文](https://github.com/golang/go/wiki/MutexOrChannel)如下：
```
Share memory by communicating, don't communicate by sharing memory.
```

很多人对这句话的理解停留在字面上，变成了不要使用共享内存(Shared Memory)。其实这句话的意思是说在需要并发处理的情况下，如果多个线程或协程需要通信，比如读写某个数据，不要使用一个内存变量(共享同一块内存)加锁的方式来实现，而应该用无锁通信的方式来实现(共享内存数据)。

在golang中，[Channel和Mutex各有适用的场景](https://blog.golang.org/share-memory-by-communicating)。
```
Channel: passing ownership of data, distributing units of work, communicating async results.
Mutex: caches, state.
```

而且[golang的map也不是协程安全的](https://golang.org/doc/faq#atomic_maps)，本质也是通过共享内存(map)来通信(协程之间同步数据)。
```
After long discussion it was decided that the typical use of maps did not require safe access from multiple goroutines, and in those cases where it did, the map was probably part of some larger data structure or computation that was already synchronized. Therefore requiring that all map operations grab a mutex would slow down most programs and add safety to few. This was not an easy decision, however, since it means uncontrolled map access can crash the program.

The language does not preclude atomic map updates. When required, such as when hosting an untrusted program, the implementation could interlock map access.
```

#### 并发和并行
并发(Concurrent)，经常说高并发服务，其实后台可能是基于epoll的单线程程序，所有请求是并行发起的，但在程序中是CPU分片处理的。
并行(Parallel)，是多个逻辑单元(CPU)同时处理多个不同的请求。
在单CPU的情况下，goroutine是并发的，正在运行的goroutine可以主动放弃CPU而切换到其他goroutine，但任何一个时候，只会有一个goroutine在运行，并不会出现类似线程同时读写资源(内存)的问题。

### 指针

#### interface

##### 规范
实现interface允许传递一个Type，也可以传递一个指针*Type。
```go
func (s *MyStruct) pointerMethod() { } // method on pointer
func (s MyStruct)  valueMethod()   { } // method on value
```

##### 示例1 
```go
package main

import (
	"fmt"
	"reflect"
)

type Pet interface {
	Speak() string
}

type Cat struct {
	name string
}

type Dog struct {
	name string
}

func (this Cat) Speak() string {
	return fmt.Sprintf("Cat(%s)", this.name)
}

func (this *Dog) Speak() string {
	return fmt.Sprintf("Dog(%s)", this.name)
}

func main() {
	c1 := Cat{name: "c1"}
	c2 := new(Cat)
	c2.name = "c2"

	d1 := Dog{name: "d1"}
	d2 := new(Dog)
	d2.name = "d2"

	var p1 Pet = Cat{name: "p1"} // recommended
	var p2 Pet = new(Cat)        // not recommended

	// Dog does not implement Pet (Speak method has pointer receiver)
	// *Dog is a Type
	// var p3 Pet = Dog{name: "p3"}
	var p3 Pet = &Dog{name: "p3"} // recommended
	var p4 Pet = new(Dog)         // not recommended

	fmt.Printf("c1, Speak: %s, type: %T, kind: %v\n", c1.Speak(), c1, reflect.TypeOf(c1).Kind())
	fmt.Printf("c2, Speak: %s, type: %T, kind: %v\n", c2.Speak(), c2, reflect.TypeOf(c2).Kind())

	fmt.Printf("d1, Speak: %s, type: %T, kind: %v\n", d1.Speak(), d1, reflect.TypeOf(d1).Kind())
	fmt.Printf("d2, Speak: %s, type: %T, kind: %v\n", d2.Speak(), d2, reflect.TypeOf(d2).Kind())

	fmt.Printf("p1, Speak: %s, type: %T, kind: %v\n", p1.Speak(), p1, reflect.TypeOf(p1).Kind())
	fmt.Printf("p2, Speak: %s, type: %T, kind: %v\n", p2.Speak(), p2, reflect.TypeOf(p2).Kind())
	fmt.Printf("p3, Speak: %s, type: %T, kind: %v\n", p3.Speak(), p3, reflect.TypeOf(p3).Kind())
	fmt.Printf("p4, Speak: %s, type: %T, kind: %v\n", p4.Speak(), p4, reflect.TypeOf(p4).Kind())
}
```

```shell
$ ./interface3 
c1, Speak: Cat(c1), type: main.Cat, kind: struct
c2, Speak: Cat(c2), type: *main.Cat, kind: ptr
d1, Speak: Dog(d1), type: main.Dog, kind: struct
d2, Speak: Dog(d2), type: *main.Dog, kind: ptr
p1, Speak: Cat(p1), type: main.Cat, kind: struct
p2, Speak: Cat(), type: *main.Cat, kind: ptr
p3, Speak: Dog(p3), type: *main.Dog, kind: ptr
p4, Speak: Dog(), type: *main.Dog, kind: ptr
```

- 实现interface的方法时，可以使用Type，也可以使用*Type；
- Type和*Type是两个不同的类型，但两者的操作基本是兼容的。

##### 示例2
```go
package main

import (
	"fmt"
)

type Counter interface {
	Add() int
}

type ValueCounter struct {
	counter int
}

func (this ValueCounter) Add() int {
	// fmt.Printf("ValueCounter, this: %p\n", &this)
	this.counter++
	return this.counter
}

type PointerCounter struct {
	counter int
}

func (this *PointerCounter) Add() int {
	// fmt.Printf("PointerCounter, this: %p\n", &this)
	this.counter++
	return this.counter
}

func main() {
	var valueCounter Counter = ValueCounter{counter: 100}
	var pointerCounter Counter = &PointerCounter{counter: 200}
	for i := 0; i < 3; i++ {
		fmt.Printf("valueCounter: %d, pointerCounter: %d\n",
			valueCounter.Add(), pointerCounter.Add())
	}
}
```

```shell
$ ./interface2 
valueCounter: 101, pointerCounter: 201
valueCounter: 101, pointerCounter: 202
valueCounter: 101, pointerCounter: 203
```

使用使用*struct的实现会修改原变量的成员，但使用struct的实现并没有修改原变量的成员。原因是interface会将Type type作为一个参数传入到实现的函数中，如果传入的是struct，则会产生一份struct的拷贝(clone)，这样修改struct成员实际是修改的拷贝对象，因而原变量的成员总保持不变。

##### 总结

- Type和*Type是两个不同的类型，但两者的操作是基本兼容的；
- interface会将Type变量作为参数拷贝一份传入到实现函数中，如果使用Type则无法通过interface修改成员变量；
- 建议实现interface的Type总是使用指针类型*Type；

golang官方说明：[Pointers and Allocation](https://golang.org/doc/faq#Pointers)。

## 测试

### 单元测试
#### 规范
golang的单元测试用例规范：

- 文件名必须是_test.go结尾；
- 测试用例函数必须是Test开头；
- 测试用例函数的参数是testing.T；

#### 示例
对于reverse_string.go中的reverseString编写一个单元测试用例：

- 新建reverse_string_test.go测试文件，测试用例函数名为TestReverseString；
- 编写测试用例，初始化的input和expected，多个测试用例使用数组初始化；
- 运行go test reverse_string*，只运行通配符匹配的测试用例文件；

```go
// reverse_string.go

package src

// ref: https://github.com/golang/example/blob/master/stringutil/reverse.go
func reverseString(s string) string {
	len := len(s)
	r := make([]byte, len)
	for i, j := 0, len-1; i < len/2; i, j = i+1, j-1 {
		r[i], r[j] = s[j], s[i]
	}
	if len%2 != 0 {
		r[len/2] = s[len/2]
	}
	return string(r)
}
```
```go
// reverse_string_test.go

package src

import "testing"

func TestReverseString(t *testing.T) {
	var cases = []struct {
		input    string
		expected string
	}{
		{"", ""},
		{"a", "a"},
		{"aa", "aa"},
		{"aba", "aba"},
		{"abba", "abba"},
		{"hello", "olleh"},
	}

	for _, c := range cases {
		actual := reverseString(c.input)
		if actual != c.expected {
			t.Errorf("actual: %s, expected: %s, input: %s",
				actual, c.expected, c.input)
		}
	}
}
```
```shell
$ go test reverse_string*
ok      command-line-arguments  0.007s
```

### 基准测试
#### 规范
golang的基准测试用例规范：

- 文件名必须是_test.go结尾；
- 测试用例函数必须是Benchmark开头；
- 测试用例函数的参数是testing.B；

#### 示例
对于reverse_string.go中的reverseString编写一个基准测试用例：

- 新建reverse_string_benchmark_test.go测试文件，测试用例函数名为BenchmarkReverseString；
- 编写测试用例，循环`b.N`次，golang会根据实际情况调整b.N的值，以便在合理的时间内取到有效的结果；
- 运行`go test reverse_string* -test.bench=".*"`，运行benchmark需要`-test.bench`参数，".*"表示运行所有benchmark；

```go
// reverse_string_benchmark_test.go

package src

import (
	"testing"
)

func BenchmarkReverseString(b *testing.B) {
	str := "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	for i := 0; i < b.N; i++ {
		reverseString(str)
	}
}
```
```shell
$ go test reverse_string* -test.bench=".*"
goos: darwin
goarch: amd64
BenchmarkReverseString-4        10000000               125 ns/op
PASS
ok      command-line-arguments  1.396s
```

## 习题
### 习题1-defer
#### 问题
如下代码输出结果是怎样的？

```go
package main

import (
	"fmt"
)

func main() {
	deferCall()
}

func deferCall() {
	defer func() { fmt.Println("hello1") }()
	defer func() { fmt.Println("hello2") }()
	panic("fault")
}
```
#### 答案
```shell
$ ./exercise1
hello2
hello1
panic: fault
```
#### 解析
1. panic首先会处理defer，然后再报告错误；
2. defer是先进后出，所以是先hello2再hello1；

### 习题2-for
#### 问题
如下代码m1和m2输出结果是怎样的？

```go
package main

import (
	"fmt"
)

type student struct {
	Name string
	Age  int
}

func main() {
	m1 := make(map[string]*student)
	m2 := make(map[string]*student)
	stus := []student{
		{Name: "zhou", Age: 24},
		{Name: "li", Age: 23},
		{Name: "wang", Age: 22},
	}

	for index, stu := range stus {
		m1[stu.Name] = &stu
		m2[stu.Name] = &stus[index]
	}

	fmt.Printf("########### m1 ###########\n")
	for key, value := range m1 {
		fmt.Printf("m1, key: %s, value: %v\n", key, value)
	}

	fmt.Printf("########### m2 ###########\n")
	for key, value := range m2 {
		fmt.Printf("m2, key: %s, value: %v\n", key, value)
	}
}
```

#### 答案
```shell
$ ./exercise2
########### m1 ###########
m1, key: zhou, value: &{wang 22}
m1, key: li, value: &{wang 22}
m1, key: wang, value: &{wang 22}
########### m2 ###########
m2, key: zhou, value: &{zhou 24}
m2, key: li, value: &{li 23}
m2, key: wang, value: &{wang 22}
```

#### 解析
for会复用循环变量(index, element)，在每轮循环中将值拷贝到循环变量中，循环变量只是值不一样，地址是相同的。

### 习题4-struct
#### 问题
如下代码输出结果是怎样的？

```go
package main

import (
	"fmt"
)

type People struct{}

func (p *People) ShowA() {
	fmt.Println("People ShowA")
	p.ShowB()
}
func (p *People) ShowB() {
	fmt.Println("People ShowB")
}

type Teacher struct {
	People
}

func (t *Teacher) ShowB() {
	fmt.Println("Teacher ShowB")
}

func main() {
	t := Teacher{}
	t.ShowA()
}
```

#### 答案
```shell
$ ./exercise4
People ShowA
People ShowB
```

#### 解析
golang没有继承或者子类，而是嵌入或者组合。golang设计上没有使用C/C++和Java的虚表设计。

```
Golang Embedding: Go does not provide the typical, type-driven notion of subclassing, but it does have the ability to “borrow” pieces of an implementation by embedding types within a struct or interface.
```

### 习题5-select
#### 问题
如下代码输出结果是怎样的？

```go
package main

import (
	"fmt"
	"runtime"
)

func main() {
	runtime.GOMAXPROCS(1)
	chan1 := make(chan int, 1)
	chan2 := make(chan string, 1)
	chan2 <- "hello"
	chan1 <- 1
	select {
	case value := <-chan1:
		fmt.Println(value)
	case value := <-chan2:
		fmt.Println(value)
	}
}
```

#### 答案
这题比较特殊，多次运行，两个分支会随机会执行。
```shell
$ ./exercise5 
1
$ ./exercise5 
hello
```

#### 解析
[golang官方文档](https://golang.org/ref/spec#Select_statements)明确说明，select在多个条件都满足的情况下，会随机的选择一个分支执行。
```
If one or more of the communications can proceed, a single one that can proceed is chosen via a uniform pseudo-random selection. Otherwise, if there is a default case, that case is chosen. If there is no default case, the "select" statement blocks until at least one of the communications can proceed.
```

### 习题6-defer
#### 问题
如下代码输出结果是怎样的？

```go
package main

import "fmt"

func calc(str string, a, b int) int {
	ret := a + b
	fmt.Printf("str: %s, a: %d, b: %d, ret: %d\n", str, a, b, ret)
	return ret
}

func main() {
	a := 1
	b := 2
	defer calc("1", a, calc("2", a, b))
	a = 0
	defer calc("3", a, calc("4", a, b))
	b = 1
}
```

#### 答案
```shell
$ ./exercise6
str: 2, a: 1, b: 2, ret: 3
str: 4, a: 0, b: 2, ret: 2
str: 3, a: 0, b: 2, ret: 2
str: 1, a: 1, b: 3, ret: 4
```

#### 解析
1. defer在调用时会立刻复制参数值；
2. defer的函数参数如果有函数，那么函数会在调用时立刻执行，并将函数的返回值拷贝为参数值，这与C/C++和Java的函数调用规范相同；
3. defer的执行顺序是先进后出(FILO)；

### 习题7-slice
#### 问题
如下代码输出结果是怎样的？

```go
package main

import "fmt"

func main() {
	s := make([]int, 5)
	fmt.Printf("s: %p, len: %d, cap: %d\n", s, len(s), cap(s))
	s = append(s, 1, 2, 3)
	fmt.Printf("s: %p, len: %d, cap: %d\n", s, len(s), cap(s))
	fmt.Println("s:", s)
}
```

#### 答案
```shell
$ ./exercise7 
s: 0xc42008e000, len: 5, cap: 5
s: 0xc420096000, len: 8, cap: 10
s: [0 0 0 0 0 1 2 3]
```

#### 解析
1. make的第二个参数是len，第三个参数是cap，cap缺省时等于len；
2. golang默认会置零初始化；
3. append是在数组尾部追加，并返回一个新数组，所以s的值也会变化；
4. append追加时，cap也会增加，但cap增加没有规范说明；

### 习题8-map
#### 问题
如下代码有什么问题？

```go
package main

import (
	"fmt"
	"sync"
)

type UserTable struct {
	m map[int]string
	sync.Mutex
}

func (this *UserTable) Add(id int, name string) {
	this.Lock()
	defer this.Unlock()
	this.m[id] = name
}

func (this *UserTable) Get(id int) string {
	if name, ok := this.m[id]; ok {
		return name
	}
	return ""
}

func main() {
	table := UserTable{m: map[int]string{}}
	count := 10000
	var wg sync.WaitGroup
	wg.Add(2)
	go func() {
		defer wg.Done()
		for i := 1; i < count; i++ {
			table.Add(i, fmt.Sprintf("id_%d", i))
		}
	}()
	go func() {
		defer wg.Done()
		for i := 1; i < count; i++ {
			table.Get(i)
		}
	}()
	wg.Wait()
}
```

#### 答案
在多核CPU下，运行会出现错误。
```shell
$ ./exercise8 
fatal error: concurrent map read and map write
```

#### 解析
map不是协程安全的，读写都必须有同步机制，这里虽然Add加锁了，但Get没有加锁，两者同时运行(多核CPU)会产生冲突。
```
If you need to read from and write to a map from concurrently executing goroutines, the accesses must be mediated by some kind of synchronization mechanism.
```

### 习题11-nil
#### 问题
如下代码输出结果是怎样的？

```go
package main

import "fmt"

type People interface {
	Show()
}

type Student struct{}

func (stu *Student) Show() {
}

func live() People {
	var stu *Student
	return stu
}

func main() {
	if live() == nil {
		fmt.Println("equal nil ")
	} else {
		fmt.Println("not equal nil")
	}
}
```

#### 答案
```shell
$ ./exercise11
not equal nil
```

#### 解析
非空interface的nil能和nil比较，但不相等；

## 参考
- [The Go Programming Language](https://golang.org/)
- [Go’s http package](https://cryptic.io/go-http/)
