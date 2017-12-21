package main

import (
	"fmt"
	"net/rpc"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

type Arith int

func main() {
	client, err := rpc.DialHTTP("tcp", "localhost:8080")
	if err != nil {
		fmt.Printf("dail fail, error: %s\n", err.Error())
		return
	}
	defer client.Close()

	coNum := 5
	ch := make(chan int, coNum)

	for i := 0; i < 10; i++ {
		ch <- 1
		// Asynchronous call
		args := &Args{100, i}
		quotient := new(Quotient)
		call := client.Go("Arith.Divide", args, quotient, nil)
		go func() {
			defer func() { <-ch }()
			done := <-call.Done
			err = done.Error
			if err != nil {
				fmt.Printf("dail fail, error: %s\n", err.Error())
				return
			}
			fmt.Printf("Arith.Divide: %d / %d = %d, %d\n", args.A, args.B, quotient.Quo, quotient.Rem)
		}()
	}

	for i := 0; i < coNum; i++ {
		ch <- 1
	}
}
