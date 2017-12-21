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
