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
