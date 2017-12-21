package main

import "fmt"

// Pet is a representation of a pet
type Pet struct {
	name string
	age  int
}

// Dog is a representation of a dog
type Dog struct {
	Pet
	strain string
}

// Show Dog
func (dog *Dog) Show() {
	fmt.Printf("Dog, name: %s, age: %d, strain: %s\n",
		dog.name, dog.age, dog.strain)
}

func main() {
	datas := []struct {
		name   string
		age    int
		strain string
	}{
		{"Wang", 18, "Chinese Rural Dog"},
		{"Lucy", 19, "Labrador"},
		{"Max", 18, "Wolfdog"},
	}

	for _, data := range datas {
		p := new(Dog)
		p.name = data.name
		p.age = data.age
		p.strain = data.strain
		p.Show()
	}
}
