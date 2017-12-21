package main

import "fmt"

type Base interface {
	Dosth()
}

type Impl1 struct{}

func (this *Impl1) Dosth() {
}

func GetImpl1() Base {
	return nil
}

func GetImpl2() Base {
	var impl *Impl1 // init with zero, also be nil
	return impl
}

type Impl4 struct{}

func (this *Impl4) Dosth() {
}

func GetImpl4() Base {
	var impl *Impl4 // init with zero, also be nil
	return impl
}

func main() {
	p1 := GetImpl1()
	p2 := GetImpl2()
	p3 := GetImpl2()
	p4 := GetImpl4()

	fmt.Println(p1 == nil) // true
	fmt.Println(p2 == nil) // false
	fmt.Println(p1 == p2)  // false
	fmt.Println(p2 == p3)  // true
	fmt.Println(p2 == p4)  // false
}
