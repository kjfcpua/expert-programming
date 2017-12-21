package main

import (
	"fmt"
)

func Producer(ch chan int) {
	for i := 1; i <= 10; i++ {
		fmt.Printf("Producer add, value: %d\n", i)
		ch <- i
	}
	close(ch)
}

func Consumer(id int, ch chan int, done chan bool) {
	for {
		value, ok := <-ch
		if ok {
			fmt.Printf("Consumer, id: %d, recv: %d\n", id, value)
		} else {
			fmt.Printf("Consumer, id: %d, closed\n", id)
			break
		}
	}
	done <- true
}

func main() {
	// if umcomment this line, what happend ?
	// runtime.GOMAXPROCS(1)

	ch := make(chan int, 3)

	coNum := 2
	done := make(chan bool, coNum)
	for i := 1; i <= coNum; i++ {
		go Consumer(i, ch, done)
	}

	go Producer(ch)

	for i := 1; i <= coNum; i++ {
		<-done
	}
}
