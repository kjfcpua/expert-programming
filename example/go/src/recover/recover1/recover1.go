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
