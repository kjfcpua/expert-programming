package main

import "fmt"

func main() {
	a1 := new([]int)        // *[]int
	a2 := make([]int, 3)    // []int
	a3 := make([]int, 3, 5) // []int

	fmt.Printf("########### a1 ###########\n")
	fmt.Printf("len(*a1): %d, cap(*a1): %d\n", len(*a1), cap(*a1))
	for i, n := range *a1 {
		fmt.Printf("a1[%d]: %d\n", i, n)
	}

	fmt.Printf("########### a2 ###########\n")
	fmt.Printf("len(a2): %d, cap(a2): %d\n", len(a2), cap(a2))
	for i, n := range a2 {
		fmt.Printf("a2[%d]: %d\n", i, n)
	}

	fmt.Printf("########### a3 ###########\n")
	fmt.Printf("len(a3): %d, cap(a3): %d\n", len(a3), cap(a3))
	for i, n := range a3 {
		fmt.Printf("a3[%d]: %d\n", i, n)
	}
}
