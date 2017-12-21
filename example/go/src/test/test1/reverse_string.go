package main

import "fmt"

// ref: https://github.com/golang/example/blob/master/stringutil/reverse.go
func reverseString(s string) string {
	len := len(s)
	r := make([]byte, len)
	for i, j := 0, len-1; i < len/2; i, j = i+1, j-1 {
		r[i], r[j] = s[j], s[i]
	}
	if len%2 != 0 {
		r[len/2] = s[len/2]
	}
	return string(r)
}

func main() {
	fmt.Println(reverseString("abc"))
}
