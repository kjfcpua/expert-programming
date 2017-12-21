package main

import (
	"database/sql"
	"fmt"
	"math/rand"
	"os"
	"strings"
	"time"
)

type Oracle struct {
	db *sql.DB
}

func (database *Oracle) Close() {
	database.db.Close()
}

func (database *Oracle) Connect(user, password, server string, port int) error {
	os.Setenv("NLS_LANG", "")
	// scott/tiger@192.168.63.100:1521/orcl
	connString := fmt.Sprintf("%s/%s@%s:%d/orcl", user, password, server, port)
	if *debug {
		fmt.Printf("connString: %s\n", connString)
	}
	db, err := sql.Open("oci8", connString)
	if err != nil {
		fmt.Printf("Connect failed: %s", err.Error())
		return err
	}
	database.db = db
	return nil
}

func (database *Oracle) CreateTable() {
	stmt, err := database.db.Prepare("create table table1(" +
		"field1 int, field2 int, " +
		"field3 decimal(10,2), field4 decimal(10,2), " +
		"field5 char(20), field6 char(200), " +
		"field7 varchar(50), field8 varchar(50), " +
		"field9 varchar(100), field10 varchar(200), " +
		"create_time timestamp(3), modify_time timestamp(3))")
	if err != nil {
		fmt.Printf("Prepare failed:", err.Error())
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec()
	if err != nil {
		fmt.Printf("Exec failed:", err.Error())
		return
	}

	fmt.Printf("CreateTable succ, table: %s\n", "table1")
}

func (database *Oracle) Count() {
	stmt, err := database.db.Prepare("select count(*) from table1")
	if err != nil {
		fmt.Printf("Prepare failed: %s", err.Error())
		return
	}
	defer stmt.Close()

	row := stmt.QueryRow()
	var count int64
	err = row.Scan(&count)
	if err != nil {
		fmt.Printf("Scan failed: %s\n", err.Error())
		return
	}
	fmt.Printf("count: %d\n", count)
}

func (database *Oracle) Query(num int) {
	db := database.db
	rows, err := db.Query("select max(field1) from table1")
	if err != nil {
		fmt.Printf("Query failed:", err.Error())
		return
	}
	defer rows.Close()
	cols, err := rows.Columns()
	if err != nil {
		fmt.Printf("Columns failed:", err.Error())
		return
	}
	if cols == nil {
		return
	}
	vals := make([]interface{}, len(cols))
	for i := 0; i < len(cols); i++ {
		vals[i] = new(interface{})
		if i != 0 {
			fmt.Print(" | ")
		}
		fmt.Print(cols[i])
	}
	fmt.Println()
	for rows.Next() {
		err = rows.Scan(vals...)
		if err != nil {
			fmt.Println(err)
			continue
		}
		for i := 0; i < len(vals); i++ {
			if i != 0 {
				fmt.Print(" | ")
			}
			PrintValue(vals[i].(*interface{}))
		}
		fmt.Println()
		num--
		if num == 0 {
			break
		}
	}
}

func (database *Oracle) Insert(num int) {
	db := database.db
	stmt, err := db.Prepare("insert into table1" +
		"(field1, field2, field3, field4, field5, field6, field7, field8, " +
		"field9, field10, create_time, modify_time) " +
		"values(:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, sysdate, sysdate)")
	if err != nil {
		fmt.Printf("Prepare failed:", err.Error())
		return
	}
	defer stmt.Close()

	var coNum = 100
	ch := make(chan int, coNum)
	rand.Seed(time.Now().UTC().UnixNano())
	for i := 0; i < num; i++ {
		ch <- 1
		go func() {
			rs, err := stmt.Exec(
				rand.Int31(), rand.Int31(), rand.Float32(), rand.Float32(),
				RandString(20), RandString(200),
				RandString(RandInt(0, 50)), RandString(RandInt(0, 50)),
				RandString(RandInt(0, 100)), RandString(RandInt(0, 200)))
			if err != nil {
				fmt.Print("Exce failed:", err.Error())
			}
			cnt, err := rs.RowsAffected()
			if err != nil || cnt == 0 {
				fmt.Print(err)
			}
			<-ch
		}()
	}
	for i := 0; i < coNum; i++ {
		ch <- 1
	}
}

func (database *Oracle) DropTable() {
	db := database.db
	stmt, err := db.Prepare("drop table table1")
	if err != nil {
		fmt.Printf("Prepare failed: ", err.Error())
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec()
	if err != nil {
		fmt.Printf("Exec failed:", err.Error())
		return
	}

	fmt.Printf("DropTable succ, table: %s\n", "table1")
}

func (database *Oracle) TestBlob() {
	db := database.db

	db.Exec("drop table blob_example")

	_, err := db.Exec("create table blob_example(id varchar2(256) not null primary key, data blob)")
	if err != nil {
		fmt.Printf("Exec failed:", err.Error())
		return
	}

	b := []byte(strings.Repeat("X", 200))
	_, err = db.Exec("insert into blob_example(id, data) values(:1, :2)", "001", b)
	if err != nil {
		fmt.Printf("Exec failed:", err.Error())
		return
	}

	rows, err := db.Query("select * from blob_example")
	if err != nil {
		fmt.Printf("Exec failed:", err.Error())
		return
	}
	defer rows.Close()

	for rows.Next() {
		var id string
		var data []byte
		rows.Scan(&id, &data)
		if string(b) != string(data) {
			panic("BLOB doesn't work correctly")
		}
		fmt.Println(id, string(data))
	}
}
