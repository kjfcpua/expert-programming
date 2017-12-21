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