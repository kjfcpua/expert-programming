package main

import (
	"errors"
	"fmt"
	"net"
	"net/http"
	"net/rpc"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

type Arith int

func (t *Arith) Multiply(args *Args, reply *int) error {
	fmt.Printf("Multiply, a: %d, b: %d\n", args.A, args.B)
	*reply = args.A * args.B
	return nil
}

func (t *Arith) Divide(args *Args, quo *Quotient) error {
	fmt.Printf("Divide, a: %d, b: %d\n", args.A, args.B)
	if args.B == 0 {
		return errors.New("divide by zero")
	}
	quo.Quo = args.A / args.B
	quo.Rem = args.A % args.B
	return nil
}

func main() {
	arith := new(Arith)
	rpc.Register(arith)
	rpc.HandleHTTP()
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		fmt.Printf("listen fail, error: %s\n", err.Error())
		return
	}
	http.Serve(listener, nil)
}
