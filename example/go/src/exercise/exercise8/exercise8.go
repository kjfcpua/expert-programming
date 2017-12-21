package main

import (
	"fmt"
	"sync"
)

type UserTable struct {
	m map[int]string
	sync.Mutex
}

func (this *UserTable) Add(id int, name string) {
	this.Lock()
	defer this.Unlock()
	this.m[id] = name
}

func (this *UserTable) Get(id int) string {
	if name, ok := this.m[id]; ok {
		return name
	}
	return ""
}

func main() {
	table := UserTable{m: map[int]string{}}
	count := 10000
	var wg sync.WaitGroup
	wg.Add(2)
	go func() {
		defer wg.Done()
		for i := 1; i < count; i++ {
			table.Add(i, fmt.Sprintf("id_%d", i))
		}
	}()
	go func() {
		defer wg.Done()
		for i := 1; i < count; i++ {
			table.Get(i)
		}
	}()
	wg.Wait()
}
