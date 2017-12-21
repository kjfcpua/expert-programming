package main

import (
	"fmt"
	"sync"
)

func main() {
	fmt.Println("main start")
	var wg sync.WaitGroup
	for i := 0; i < 3; i++ {
		wg.Add(1)
		fmt.Printf("go: %d\n", i)
		go func(n int) {
			defer fmt.Printf("exit: %d\n", n)
			defer wg.Done()
			fmt.Printf("start: %d\n", n)
		}(i)
	}
	wg.Wait()
	fmt.Println("main exit")
}
