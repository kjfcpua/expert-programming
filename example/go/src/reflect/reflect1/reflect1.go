package main

import (
	"errors"
	"fmt"
	"reflect"
)

type Pig struct {
	name string
}

type Database interface {
	Show()
}

type Oracle struct {
}

func (database *Oracle) Show() {
	fmt.Printf("Oracle\n")
}

func main() {
	var b1 bool = true                   // bool
	var int1 int = 1                     // int
	var int2 int32 = 2                   // int32
	var int3 uint32 = 4                  // uint32
	var int4 uintptr = 7                 // uintptr
	var float1 float32 = 3.14            // float32
	var float2 float64 = 6.18            // float64
	var c1 byte = 'x'                    // uint8
	var c2 rune = 'z'                    // int32
	var str string = "hello"             // string
	var err1 error = nil                 // <nil>
	var err2 error = errors.New("fail")  // *errors.errorString, ptr
	var pig1 Pig                         // main.Pig
	pig2 := new(Pig)                     // *main.Pig, ptr
	var database1 Database               // <nil>
	var database2 Database = new(Oracle) // *main.Oracle, ptr
	ch1 := make(chan int, 3)             // chan int
	var array1 [3]int                    // [3]int, array
	var array2 [3][2]int                 // [3][2]int, array
	var s1 []int                         // []int, slice
	map1 := make(map[int]int)            // map[int]int
	map2 := make(map[int]string)         // map[int]string

	fmt.Printf("b1, type: %s, kind: %s\n", reflect.TypeOf(b1), reflect.TypeOf(b1).Kind())
	fmt.Printf("int1, type: %s, kind: %s\n", reflect.TypeOf(int1), reflect.TypeOf(int1).Kind())
	fmt.Printf("int2, type: %s, kind: %s\n", reflect.TypeOf(int2), reflect.TypeOf(int2).Kind())
	fmt.Printf("int3, type: %s, kind: %s\n", reflect.TypeOf(int3), reflect.TypeOf(int3).Kind())
	fmt.Printf("int4, type: %s, kind: %s\n", reflect.TypeOf(int4), reflect.TypeOf(int4).Kind())
	fmt.Printf("float1, type: %s, kind: %s\n", reflect.TypeOf(float1), reflect.TypeOf(float1).Kind())
	fmt.Printf("float2, type: %s, kind: %s\n", reflect.TypeOf(float2), reflect.TypeOf(float2).Kind())
	fmt.Printf("c1, type: %s, kind: %s\n", reflect.TypeOf(c1), reflect.TypeOf(c1).Kind())
	fmt.Printf("c2, type: %s, kind: %s\n", reflect.TypeOf(c2), reflect.TypeOf(c2).Kind())
	fmt.Printf("str, type: %s, kind: %s\n", reflect.TypeOf(str), reflect.TypeOf(str).Kind())
	fmt.Println("err1: ", reflect.TypeOf(err1))
	fmt.Printf("err2, type: %s, kind: %s\n", reflect.TypeOf(err2), reflect.TypeOf(err2).Kind())
	fmt.Printf("pig1, type: %s, kind: %s\n", reflect.TypeOf(pig1), reflect.TypeOf(pig1).Kind())
	fmt.Printf("pig2, type: %s, kind: %s\n", reflect.TypeOf(pig2), reflect.TypeOf(pig2).Kind())
	fmt.Println("database1: ", reflect.TypeOf(database1))
	fmt.Printf("database2, type: %s, kind: %s\n", reflect.TypeOf(database2), reflect.TypeOf(database2).Kind())
	fmt.Printf("ch1, type: %s, kind: %s\n", reflect.TypeOf(ch1), reflect.TypeOf(ch1).Kind())
	fmt.Printf("array1, type: %s, kind: %s\n", reflect.TypeOf(array1), reflect.TypeOf(array1).Kind())
	fmt.Printf("array2, type: %s, kind: %s\n", reflect.TypeOf(array2), reflect.TypeOf(array2).Kind())
	fmt.Printf("s1, type: %s, kind: %s\n", reflect.TypeOf(s1), reflect.TypeOf(s1).Kind())
	fmt.Printf("map1, type: %s, kind: %s\n", reflect.TypeOf(map1), reflect.TypeOf(map1).Kind())
	fmt.Printf("map2, type: %s, kind: %s\n", reflect.TypeOf(map2), reflect.TypeOf(map2).Kind())
}
