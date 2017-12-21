package main

import (
	"fmt"
)

type student struct {
	Name string
	Age  int
}

func main() {
	m1 := make(map[string]*student)
	m2 := make(map[string]*student)
	stus := []student{
		{Name: "zhou", Age: 24},
		{Name: "li", Age: 23},
		{Name: "wang", Age: 22},
	}

	for index, stu := range stus {
		m1[stu.Name] = &stu
		m2[stu.Name] = &stus[index]
	}

	fmt.Printf("########### m1 ###########\n")
	for key, value := range m1 {
		fmt.Printf("m1, key: %s, value: %v\n", key, value)
	}

	fmt.Printf("########### m2 ###########\n")
	for key, value := range m2 {
		fmt.Printf("m2, key: %s, value: %v\n", key, value)
	}
}
