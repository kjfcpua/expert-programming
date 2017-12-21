package main

import (
	"fmt"
	"net"

	proto "../proto"

	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

type server struct{}

func (s *server) Multiply(ctx context.Context, request *proto.MultiplyRequest) (response *proto.MultiplyResponse, err error) {
	// fmt.Printf("gRPC Multiply, request.X: %d, request.Y: %d\n", request.X, request.Y)
	response = &proto.MultiplyResponse{Result: request.X * request.Y}
	return
}

func main() {
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		fmt.Printf("listen fail, error: %s\n", err.Error())
		return
	}
	gsvr := grpc.NewServer()
	defer gsvr.Stop()
	proto.RegisterArithServer(gsvr, &server{})
	gsvr.Serve(listener)
}
