package main

import "fmt"

func main() {
	s := make([]int, 5)
	fmt.Printf("s: %p, len: %d, cap: %d\n", s, len(s), cap(s))
	s = append(s, 1, 2, 3)
	fmt.Printf("s: %p, len: %d, cap: %d\n", s, len(s), cap(s))
	fmt.Println("s:", s)
}
