package main

import (
	"fmt"
	"runtime"
	"sync"
)

func main() {
	runtime.GOMAXPROCS(1)
	wg := sync.WaitGroup{}
	wg.Add(2000)
	for i := 0; i < 1000; i++ {
		go func() {
			fmt.Println("A, i: ", i)
			wg.Done()
		}()
	}
	for i := 0; i < 1000; i++ {
		go func(i int) {
			fmt.Println("B, i: ", i)
			wg.Done()
		}(i)
	}
	wg.Wait()
}
