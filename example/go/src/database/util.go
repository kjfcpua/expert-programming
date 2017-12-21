package main

import (
	"fmt"
	"math/rand"
	"time"
)

func RandInt(min int32, max int32) int32 {
	return min + (rand.Int31() % (max - min))
}

func RandString(n int32) string {
	const letterBytes = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	b := make([]byte, n)
	for i := range b {
		b[i] = letterBytes[rand.Int63()%int64(len(letterBytes))]
	}
	return string(b)
}

func PrintValue(pval *interface{}) {
	switch v := (*pval).(type) {
	case nil:
		fmt.Print("NULL")
	case bool:
		if v {
			fmt.Print("1")
		} else {
			fmt.Print("0")
		}
	case []byte:
		fmt.Print(string(v))
	case time.Time:
		// http://fuckinggodateformat.com/
		fmt.Print(v.Format("2006-01-02 15:04:05.000"))
	default:
		fmt.Print(v)
	}
}
