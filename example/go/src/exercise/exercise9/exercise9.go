package main

import "fmt"
import "sync"

type ThreadSafeSet struct {
	sync.RWMutex
	datas []int
}

func (set *ThreadSafeSet) Iter() <-chan interface{} {
	ch := make(chan interface{})
	go func() {
		set.RLock()

		for elem := range set.datas {
			fmt.Printf("send, value: %v\n", elem)
			ch <- elem
		}

		close(ch)
		set.RUnlock()
	}()
	return ch
}

func main() {
	set := ThreadSafeSet{}
	set.datas = []int{1, 2, 3, 4, 5}
	ch := set.Iter()
	for {
		select {
		case v, ok := <-ch:
			if ok {
				fmt.Printf("recv, value: %v\n", v)
			} else {
				return
			}
		}
	}
}
