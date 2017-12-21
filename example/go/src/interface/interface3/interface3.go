package main

import (
	"fmt"
	"reflect"
)

type Pet interface {
	Speak() string
}

type Cat struct {
	name string
}

type Dog struct {
	name string
}

func (this Cat) Speak() string {
	return fmt.Sprintf("Cat(%s)", this.name)
}

func (this *Dog) Speak() string {
	return fmt.Sprintf("Dog(%s)", this.name)
}

func main() {
	c1 := Cat{name: "c1"}
	c2 := new(Cat)
	c2.name = "c2"

	d1 := Dog{name: "d1"}
	d2 := new(Dog)
	d2.name = "d2"

	var p1 Pet = Cat{name: "p1"} // recommended
	var p2 Pet = new(Cat)        // not recommended

	// Dog does not implement Pet (Speak method has pointer receiver)
	// *Dog is a Type
	// var p3 Pet = Dog{name: "p3"}
	var p3 Pet = &Dog{name: "p3"} // recommended
	var p4 Pet = new(Dog)         // not recommended

	fmt.Printf("c1, Speak: %s, type: %T, kind: %v\n", c1.Speak(), c1, reflect.TypeOf(c1).Kind())
	fmt.Printf("c2, Speak: %s, type: %T, kind: %v\n", c2.Speak(), c2, reflect.TypeOf(c2).Kind())

	fmt.Printf("d1, Speak: %s, type: %T, kind: %v\n", d1.Speak(), d1, reflect.TypeOf(d1).Kind())
	fmt.Printf("d2, Speak: %s, type: %T, kind: %v\n", d2.Speak(), d2, reflect.TypeOf(d2).Kind())

	fmt.Printf("p1, Speak: %s, type: %T, kind: %v\n", p1.Speak(), p1, reflect.TypeOf(p1).Kind())
	fmt.Printf("p2, Speak: %s, type: %T, kind: %v\n", p2.Speak(), p2, reflect.TypeOf(p2).Kind())
	fmt.Printf("p3, Speak: %s, type: %T, kind: %v\n", p3.Speak(), p3, reflect.TypeOf(p3).Kind())
	fmt.Printf("p4, Speak: %s, type: %T, kind: %v\n", p4.Speak(), p4, reflect.TypeOf(p4).Kind())
}
