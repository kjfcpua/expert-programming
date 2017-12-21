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
