package main

import (
	"errors"
	"fmt"
)

type File struct {
	name string
}

func (file *File) Open(name string) error {
	if len(name) == 0 {
		return errors.New("empty file name")
	}
	file.name = name
	fmt.Printf("Open file succ, name: %s\n", file.name)
	return nil
}

func (file *File) Read() string {
	fmt.Printf("Read file succ, name: %s\n", file.name)
	return "abcd1234"
}

func (file *File) Close() {
	fmt.Printf("Close file succ, name: %s\n", file.name)
}

func DoIO(name string) (string, error) {
	defer fmt.Printf("DoIO done, name: %s\n", name)
	file := new(File)
	err := file.Open(name)
	if err != nil {
		fmt.Printf("Open file fail, name: %s\n", name)
		return "", err
	}
	defer file.Close()
	return file.Read(), nil
}

func main() {
	datas := []struct {
		name string
	}{
		{"Readme.txt"},
		{""},
	}

	for _, data := range datas {
		cont, err := DoIO(data.name)
		if err != nil {
			fmt.Printf("DoIO fail, error: %s, name: %s\n", err.Error(), data.name)
		} else {
			fmt.Printf("DoIO succ, cont: %s, name: %s\n", cont, data.name)
		}
	}
}
