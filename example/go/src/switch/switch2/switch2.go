package main

import (
	"fmt"
)

func main() {
	num := 3
	switch {
	case 1 >= num:
		fmt.Println("num >= 1")
	case 3 >= num:
		fmt.Println("num >= 3")
	case 5 >= num:
		fmt.Println("num >= 5")
	default:
		fmt.Println("num is not deal")
	}
}
