package main

import (
	"fmt"
)

func main() {
	fmt.Println("a")
	panic("fault")
	fmt.Println("b")
}
