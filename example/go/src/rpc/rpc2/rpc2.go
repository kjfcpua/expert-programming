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

	for i := 0; i < 10; i++ {
		// Synchronous call
		args := &Args{i, i + 1}
		var reply int
		err = client.Call("Arith.Multiply", args, &reply)
		if err != nil {
			fmt.Printf("RPC fail, error: %s\n", err.Error())
			return
		}
		fmt.Printf("Arith.Multiply: %d * %d = %d\n", args.A, args.B, reply)
	}
}
