package main

import (
	"errors"
	"fmt"
)

func divide(dividend, divisor int) (int, error) {
	if divisor == 0 {
		return 0, errors.New("divide by zero")
	}
	return dividend / divisor, nil
}

func main() {
	datas := []struct {
		dividend int
		divisor  int
	}{
		{4, 2},
		{4, 1},
		{4, 0},
	}

	for _, data := range datas {
		quotient, err := divide(data.dividend, data.divisor)
		if err != nil {
			fmt.Printf("divide fail, error: %s\n", err.Error())
			return
		}
		fmt.Printf("divide succ, quotient: %d\n", quotient)
	}
}
