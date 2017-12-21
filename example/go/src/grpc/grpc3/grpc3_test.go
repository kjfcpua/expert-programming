package main

import (
	"fmt"
	"testing"

	proto "../proto"

	"golang.org/x/net/context"

	"google.golang.org/grpc"
)

func MakeClient() proto.ArithClient {
	conn, err := grpc.Dial("localhost:8080", grpc.WithInsecure())
	if err != nil {
		fmt.Printf("dail fail, error: %s\n", err.Error())
		return nil
	}
	client := proto.NewArithClient(conn)
	return client
}

func MultiplyByOneClient(client proto.ArithClient) {
	for i := 0; i < 10; i++ {
		request := &proto.MultiplyRequest{
			X: uint32(i),
			Y: uint32(i + 1),
		}
		_, err := client.Multiply(context.Background(), request)
		if err != nil {
			fmt.Printf("RPC fail, error: %s\n", err.Error())
			return
		}
	}
}

func BenchmarkMultiplyMultiClient(b *testing.B) {
	for i := 0; i < b.N; i++ {
		client := MakeClient()
		MultiplyByOneClient(client)
	}
}

func BenchmarkMultiplyByOneClient(b *testing.B) {
	client := MakeClient()
	for i := 0; i < b.N; i++ {
		MultiplyByOneClient(client)
	}
}
