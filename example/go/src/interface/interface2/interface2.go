package main

import (
	"fmt"
)

type Counter interface {
	Add() int
}

type ValueCounter struct {
	counter int
}

func (this ValueCounter) Add() int {
	// fmt.Printf("ValueCounter, this: %p\n", &this)
	this.counter++
	return this.counter
}

type PointerCounter struct {
	counter int
}

func (this *PointerCounter) Add() int {
	// fmt.Printf("PointerCounter, this: %p\n", &this)
	this.counter++
	return this.counter
}

func main() {
	var valueCounter Counter = ValueCounter{counter: 100}
	var pointerCounter Counter = &PointerCounter{counter: 200}
	for i := 0; i < 3; i++ {
		fmt.Printf("valueCounter: %d, pointerCounter: %d\n",
			valueCounter.Add(), pointerCounter.Add())
	}
}
