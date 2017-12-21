package main

import (
	"fmt"
)

func main() {
	typeof := func(v interface{}) {
		switch v.(type) {
		case bool:
			fmt.Println("bool")
		case int:
			fmt.Println("int")
		case string:
			fmt.Println("string")
		default:
			fmt.Println("unknow")
		}
	}
	typeof(true)
	typeof(0)
	typeof(-1)
	typeof("hello")
	typeof(1.0)
}
