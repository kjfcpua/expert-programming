package main

import "fmt"

func main() {
	a1 := make([]int, 3, 5)
	fmt.Printf("a1: %p, len: %d, cap: %d\n", a1, len(a1), cap(a1))
	fmt.Println("a1:", a1)

	a2 := append(a1, 4, 5)
	fmt.Printf("a2: %p, len: %d, cap: %d\n", a2, len(a2), cap(a2))
	fmt.Println("a2:", a2)

	a3 := append(a1, 4, 5, 6)
	fmt.Printf("a3: %p, len: %d, cap: %d\n", a3, len(a3), cap(a3))
	fmt.Println("a3:", a3)

	fmt.Printf("a1: %p, len: %d, cap: %d\n", a1, len(a1), cap(a1))
	fmt.Println("a1:", a1)
}
