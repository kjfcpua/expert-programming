package main

import "fmt"
import "reflect"

type People interface {
	Speak(string) string
}

type Stduent struct {
	name string
}

func (this Stduent) Speak(think string) (talk string) {
	talk = fmt.Sprintf("Student(%s) say: %s", this.name, think)
	return
}

func main() {
	p1 := Stduent{name: "a"}
	p2 := new(Stduent)
	p2.name = "b"
	var p3 People = &Stduent{name: "c"}
	var p4 People = new(Stduent)

	fmt.Println(p1.Speak("1"))
	fmt.Println(p2.Speak("2"))
	fmt.Println(p3.Speak("3"))
	fmt.Println(p4.Speak("4"))

	fmt.Printf("p1, type: %T, kind: %v\n", p1, reflect.TypeOf(p1).Kind())
	fmt.Printf("p2, type: %T, kind: %v\n", p2, reflect.TypeOf(p2).Kind())
	fmt.Printf("p3, type: %T, kind: %v\n", p3, reflect.TypeOf(p3).Kind())
	fmt.Printf("p4, type: %T, kind: %v\n", p4, reflect.TypeOf(p4).Kind())
}
