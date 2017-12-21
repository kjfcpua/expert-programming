package main

import (
	"fmt"
)

func main() {
	num := 3
	switch num {
	case 1:
		fmt.Println("num is 1")
	case 3:
		fmt.Println("num is 3")
	case 5:
		fmt.Println("num is 5")
	default:
		fmt.Println("num is not deal")
	}
}
