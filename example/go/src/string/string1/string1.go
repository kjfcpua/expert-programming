package main

import (
	"fmt"
)

func main() {
	s1 := "12345"
	c1 := []byte(s1) // string --> []bytes
	c1[0] = 'c'
	s2 := string(c1) // []bytes --> string
	fmt.Printf("s1: %s, s2: %s\n", s1, s2)
}
