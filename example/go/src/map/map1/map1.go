package main

import (
	"fmt"
)

func main() {
	map1 := map[int]string{1: "1", 2: "2"}
	map2 := make(map[int]string)
	map2[1] = "a"

	fmt.Printf("########### init ###########\n")
	fmt.Printf("len(map1): %d, len(map2): %d\n", len(map1), len(map2))
	for key, value := range map1 {
		fmt.Printf("map1[%d]: %s\n", key, value)
	}
	for key, value := range map2 {
		fmt.Printf("map1[%d]: %s\n", key, value)
	}

	fmt.Printf("########### modify ###########\n")
	map1[1] = "x"
	fmt.Printf("map1[%d]: %s\n", 1, map1[1])

	fmt.Printf("########### delete ###########\n")
	delete(map1, 1)
	fmt.Printf("map1[%d]: %s\n", 1, map1[1])
}
