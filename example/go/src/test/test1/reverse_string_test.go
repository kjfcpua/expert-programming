package main

import "fmt"
import "testing"

func TestReverseString(t *testing.T) {
	fmt.Println("TestReverseString start")

	var cases = []struct {
		input    string
		expected string
	}{
		{"", ""},
		{"a", "a"},
		{"aa", "aa"},
		{"aba", "aba"},
		{"abba", "abba"},
		{"hello", "olleh"},
	}

	for _, c := range cases {
		actual := reverseString(c.input)
		if actual != c.expected {
			t.Errorf("actual: %s, expected: %s, input: %s",
				actual, c.expected, c.input)
		}
	}
}

func TestDemo(t *testing.T) {
	t.Fatal("TestDemo start")
}
