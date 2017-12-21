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
