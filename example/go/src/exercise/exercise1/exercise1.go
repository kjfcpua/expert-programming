package main

import (
	"fmt"
)

func main() {
	deferCall()
}

func deferCall() {
	defer func() { fmt.Println("hello1") }()
	defer func() { fmt.Println("hello2") }()
	panic("fault")
}
