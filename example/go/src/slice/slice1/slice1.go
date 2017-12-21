package main

import (
	"fmt"
	"reflect"
	"unsafe"
)

func main() {
	var s1 []int32               // len: 0, cap: 0
	s2 := []int32{1, 2, 3, 4, 5} // len: 5, cap: 5
	s3 := []int32{2: 12}         // len: 3, cap: 3

	// [start, end), include start, exclude end
	// len: end - start
	// cap: cap - start
	// slice can also init from array
	s4 := s2[:]   // len: 5, cap: 5
	s5 := s2[1:]  // len: 4, cap: 4
	s6 := s2[:2]  // len: 2, cap: 5
	s7 := s2[2:4] // len: 2, cap: 3

	s8 := new([5]int)[0:3]    // len: 3, cap: 5
	s9 := make([]int32, 3, 5) // len: 3, cap: 5

	fmt.Printf("sizeof(int32): %d\n", unsafe.Sizeof((int32)(0)))

	fmt.Printf("s1, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s1), reflect.TypeOf(s1).Kind(), len(s1), cap(s1), unsafe.Sizeof(s1))
	fmt.Printf("s2, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s2), reflect.TypeOf(s2).Kind(), len(s2), cap(s2), unsafe.Sizeof(s2))
	fmt.Printf("s3, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s3), reflect.TypeOf(s3).Kind(), len(s3), cap(s3), unsafe.Sizeof(s3))

	fmt.Printf("s4, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s4), reflect.TypeOf(s4).Kind(), len(s4), cap(s4), unsafe.Sizeof(s4))
	fmt.Printf("s5, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s5), reflect.TypeOf(s5).Kind(), len(s5), cap(s5), unsafe.Sizeof(s5))
	fmt.Printf("s6, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s6), reflect.TypeOf(s6).Kind(), len(s6), cap(s6), unsafe.Sizeof(s6))
	fmt.Printf("s7, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s7), reflect.TypeOf(s7).Kind(), len(s7), cap(s7), unsafe.Sizeof(s7))

	fmt.Printf("s8, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s8), reflect.TypeOf(s8).Kind(), len(s8), cap(s8), unsafe.Sizeof(s8))
	fmt.Printf("s9, type: %s, kind: %s, len: %d, cap: %d, size: %d\n", reflect.TypeOf(s9), reflect.TypeOf(s9).Kind(), len(s9), cap(s9), unsafe.Sizeof(s9))
}
