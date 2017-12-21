package main

type Database interface {
	Connect(user, password, server string, port int) error
	Close()
	CreateTable()
	Count()
	Query(num int)
	Insert(num int)
	DropTable()
	TestBlob()
}
