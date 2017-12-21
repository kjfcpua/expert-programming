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
