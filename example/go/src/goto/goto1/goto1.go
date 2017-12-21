package main

import (
	"fmt"
)

func main() {
	arr := []int{1, 3, 4, 5}
	for _, num := range arr {
		if num%2 == 0 {
			fmt.Println(num, "is odd")
			goto END
		} else {
			fmt.Println(num, "is even")
		}
	}

END:
	fmt.Println("end")
}
