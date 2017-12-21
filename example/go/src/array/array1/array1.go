package main

import (
	"fmt"
	"reflect"
	"unsafe"
)

func main() {
	var a1 [3]int32
	var a2 [3][2]int32
	a3 := [3]int32{1, 2}
	a4 := [...]int32{1, 2, 3, 4}
	a5 := [3]int32{2: 12}
	a6 := [...]int32{3: 13}

	// not an expression
	// a1 := [3]uint32
	// a2 := [3][2]uint32

	// array index out of bounds
	// a3 := [3]uint32{1, 2, 3, 4}
	// a5 := [3]uint32{3: 13}

	fmt.Printf("sizeof(int32): %d\n", unsafe.Sizeof((int32)(0)))

	fmt.Printf("a1, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a1), reflect.TypeOf(a1).Kind(), len(a1), cap(a1), unsafe.Sizeof(a1))
	fmt.Printf("a2, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a2), reflect.TypeOf(a1).Kind(), len(a2), cap(a2), unsafe.Sizeof(a2))
	fmt.Printf("a3, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a3), reflect.TypeOf(a1).Kind(), len(a3), cap(a3), unsafe.Sizeof(a3))
	fmt.Printf("a4, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a4), reflect.TypeOf(a1).Kind(), len(a4), cap(a4), unsafe.Sizeof(a4))
	fmt.Printf("a5, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a5), reflect.TypeOf(a1).Kind(), len(a5), cap(a5), unsafe.Sizeof(a5))
	fmt.Printf("a6, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(a6), reflect.TypeOf(a1).Kind(), len(a6), cap(a6), unsafe.Sizeof(a6))
}
