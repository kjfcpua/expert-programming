package main

func ChanSendAndRecv() int {
	ch := make(chan int, 1)
	ch <- 1
	val := <-ch
	return val
}
