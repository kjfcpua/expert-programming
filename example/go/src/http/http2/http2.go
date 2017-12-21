package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	resp, err := http.Get("http://localhost:8080/home")
	if err != nil {
		fmt.Printf("GET fail, error: %s\n", err.Error())
		return
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("read body fail, error: %s\n", err.Error())
		return
	}
	fmt.Printf("########### content ###########\n")
	fmt.Printf("%s", body)
	fmt.Printf("###########   end   ###########\n")
}
