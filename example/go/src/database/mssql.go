package main

import (
	"database/sql"
	"fmt"
	"math/rand"
	"time"
)

type Mssql struct {
	db *sql.DB
}

func (database *Mssql) Connect(user, password, server string, port int) error {
	// should not remove blank in conn str
	connString := fmt.Sprintf("server=%s;user id=%s;password=%s;port=%d",
		server, user, password, port)
	if *debug {
		fmt.Printf("connString:%s\n", connString)
	}
	db, err := sql.Open("mssql", connString)
	if err != nil {
		fmt.Printf("Connect failed: %s", err.Error())
		return err
	}
	database.db = db
	return nil
}

func (database *Mssql) Close() {
	database.db.Close()
}

func (database *Mssql) CreateTable() {
	stmt, err := database.db.Prepare("create table db1.dbo.table1(" +
		"field1 int, field2 int, " +
		"field3 decimal(10,2), field4 decimal(10,2), " +
		"field5 char(20), field6 char(200), " +
		"field7 varchar(50), field8 varchar(50), " +
		"field9 varchar(100), field10 varchar(200), " +
		"create_time datetime, modify_time datetime)")
	if err != nil {
		fmt.Printf("Prepare failed: %s\n", err.Error())
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec()
	if err != nil {
		fmt.Printf("Exec failed: %s\n", err.Error())
		return
	}

	fmt.Printf("CreateTable succ, table: %s\n", "db1.dbo.table1")
}

func (database *Mssql) Count() {
	stmt, err := database.db.Prepare("select count(*) from db1.dbo.table1")
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

func (database *Mssql) Query(num int) {
	db := database.db
	rows, err := db.Query("select max(field1) from db1.dbo.table1")
	if err != nil {
		fmt.Printf("Query failed: %s\n", err.Error())
		return
	}
	defer rows.Close()
	cols, err := rows.Columns()
	if err != nil {
		fmt.Printf("Columns failed: %s\n", err.Error())
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

func (database *Mssql) Insert(num int) {
	db := database.db
	stmt, err := db.Prepare("insert into db1.dbo.table1" +
		"(field1, field2, field3, field4, field5, field6, field7, field8, " +
		"field9, field10, create_time, modify_time) " +
		"values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, SYSDATETIME(), SYSDATETIME())")
	if err != nil {
		fmt.Printf("Prepare failed: %s\n", err.Error())
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
				fmt.Print("Scan failed:", err.Error())
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

func (database *Mssql) DropTable() {
	db := database.db
	stmt, err := db.Prepare("drop table db1.dbo.table1")
	if err != nil {
		fmt.Printf("Prepare failed: %s\n", err.Error())
		return
	}
	defer stmt.Close()

	_, err = stmt.Exec()
	if err != nil {
		fmt.Printf("Exec failed: %s\n", err.Error())
		return
	}

	fmt.Printf("DropTable succ, table: %s", "db1.dbo.table1")
}

func (database *Mssql) TestBlob() {
	fmt.Printf("TODO\n")
}
