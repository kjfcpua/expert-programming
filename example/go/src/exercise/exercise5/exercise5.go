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
