package main

import (
	"fmt"
	"runtime"
)

func main() {
	fmt.Println(runtime.NumCPU())
	n1 := runtime.GOMAXPROCS(runtime.NumCPU())
	n2 := runtime.GOMAXPROCS(n1)
	fmt.Printf("GOMAXPROCS, default: %d, cpunum: %d\n", n1, n2)
}
