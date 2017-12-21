package main

import (
	"fmt"
)

func divide(dividend, divisor int) (quotient float32, remainder int) {
	quotient = (float32)(dividend) / (float32)(divisor)
	remainder = dividend % divisor
	return
}

func main() {
	datas := []struct {
		dividend int
		divisor  int
	}{
		{4, 3},
		{4, 2},
	}

	for _, data := range datas {
		quotient, remainder := divide(data.dividend, data.divisor)
		fmt.Printf("divide(%d, %d), quotient: %f, remainder: %d\n",
			data.dividend, data.divisor, quotient, remainder)
	}
}
