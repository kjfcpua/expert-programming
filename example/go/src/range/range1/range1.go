package main

import (
	"fmt"
)

func main() {
	array := [...]int{1, 2, 3, 4}
	slice := []int{1, 2, 3, 4, 5}
	map1 := map[int]int{1: 11, 2: 12, 3: 13}

	fmt.Printf("########### array ###########\n")
	for index, elem := range array {
		fmt.Printf("index: %d, elem: %d, &index: %p, &elem: %p\n", index, elem, &index, &elem)
	}

	fmt.Printf("########### slice ###########\n")
	for index, elem := range slice {
		fmt.Printf("index: %d, elem: %d, &index: %p, &elem: %p\n", index, elem, &index, &elem)
	}

	fmt.Printf("########### str ###########\n")
	for index, elem := range "hello" {
		fmt.Printf("index: %d, elem: %c, &index: %p, &elem: %p\n", index, elem, &index, &elem)
	}

	fmt.Printf("########### map1 ###########\n")
	for key, value := range map1 {
		fmt.Printf("key: %d, value: %d, &key: %p, &value: %p\n", key, value, &key, &value)
	}

	fmt.Printf("########### map1 ###########\n")
	for key := range map1 {
		fmt.Printf("key: %d, &key: %p\n", key, &key)
	}
}
