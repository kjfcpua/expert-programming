package main

import (
	"fmt"

	proto "../proto"

	"golang.org/x/net/context"

	"google.golang.org/grpc"
)

func main() {
	conn, err := grpc.Dial("localhost:8080", grpc.WithInsecure())
	if err != nil {
		fmt.Printf("dail fail, error: %s\n", err.Error())
		return
	}
	defer conn.Close()

	client := proto.NewArithClient(conn)

	for i := 0; i < 10; i++ {
		request := &proto.MultiplyRequest{
			X: uint32(i),
			Y: uint32(i + 1),
		}
		response, err := client.Multiply(context.Background(), request)
		if err != nil {
			fmt.Printf("RPC fail, error: %s\n", err.Error())
			return
		}
		fmt.Printf("Arith.Multiply: %d * %d = %d\n", request.X, request.Y, response.Result)
	}
}
