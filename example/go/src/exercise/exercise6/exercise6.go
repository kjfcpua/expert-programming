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
