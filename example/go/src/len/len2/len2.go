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
