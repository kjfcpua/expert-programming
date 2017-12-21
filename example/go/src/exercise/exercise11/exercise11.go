package main

import "fmt"

type People interface {
	Show()
}

type Student struct{}

func (stu *Student) Show() {
}

func live() People {
	// var stu *Student
	return nil
}

func main() {
	if live() == nil {
		fmt.Println("equal nil ")
	} else {
		fmt.Println("not equal nil")
	}
}
