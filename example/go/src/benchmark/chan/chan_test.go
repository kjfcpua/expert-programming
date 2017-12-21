package main

import "testing"

func BenchmarkChanSendAndRecv(b *testing.B) {
	for i := 0; i < b.N; i++ {
		ChanSendAndRecv()
	}
}
