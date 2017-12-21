package main

import (
	"flag"
	"fmt"

	_ "github.com/denisenkom/go-mssqldb"
	_ "github.com/mattn/go-oci8"
)

var (
	debug         = flag.Bool("debug", false, "enable debugging")
	command       = flag.String("command", "", "command to run {count, query, insert}")
	driver        = flag.String("driver", "oci8", "{mssql, oci8}")
	user          = flag.String("user", "scott", "the database user{sa, scott}") // sa
	password      = flag.String("password", "tiger", "the database password")
	server        = flag.String("server", "192.168.63.100", "the database server")
	port     *int = flag.Int("port", 1521, "the database port")
	num      *int = flag.Int("num", 1, "the num of row")
)

func main() {
	flag.Parse()

	if *debug {
		fmt.Printf("command: %s\n", *command)
		fmt.Printf("driver: %s\n", *driver)
		fmt.Printf("user: %s\n", *user)
		fmt.Printf("password: %s\n", *password)
		fmt.Printf("server: %s\n", *server)
		fmt.Printf("port: %d\n", *port)
		fmt.Printf("num: %d\n", *num)
	}

	var database Database
	switch *driver {
	case "mssql":
		database = new(Mssql)
	case "oci8":
		database = new(Oracle)
	default:
		fmt.Printf("Driver [%s] not handle", *driver)
		return
	}
	err := database.Connect(*user, *password, *server, *port)
	if err != nil {
		fmt.Printf("Connect failed: %s", err.Error())
		return
	}
	defer database.Close()

	switch *command {
	case "test_blob":
		database.TestBlob()
	case "create_table":
		database.CreateTable()
	case "count":
		database.Count()
	case "query":
		database.Query(*num)
	case "insert":
		database.Insert(*num)
	case "drop_table":
		database.DropTable()
	default:
		fmt.Printf("Command [%s] not found", *command)
	}
}
