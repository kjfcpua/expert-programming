package main

import (
	"fmt"
)

// Database is interface
type Database interface {
	Show()
}

// Mssql is a representation of a mssql
type Mssql struct {
}

// Show for Mssql implement Database
func (this *Mssql) Show() {
	fmt.Printf("Mssql\n")
}

// Oracle is a representation of a oracle
type Oracle struct {
}

// Show for Oracle implement Database
func (this *Oracle) Show() {
	fmt.Printf("Oracle\n")
}

func main() {
	var d1 Database = new(Mssql)
	var d2 Database = new(Oracle)
	d1.Show()
	d2.Show()
}
