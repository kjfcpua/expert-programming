package main

import (
	"fmt"
	"time"
)

const (
    RFC822Format  = "Mon, 02 Jan 2006 15:04:05 MST"
    ISO8601Format = "2006-01-02T15:04:05Z"
)

func NowUTCSeconds() int64 { return time.Now().UTC().Unix() }

func NowUTCNanoSeconds() int64 { return time.Now().UTC().UnixNano() }

func FormatRFC822Date(timestamp_second int64) string {
    tm := time.Unix(timestamp_second, 0).UTC()
    return tm.Format(RFC822Format)
}

func ParseRFC822Date(time_string string) (time.Time, error) {
    return time.Parse(RFC822Format, time_string)
}

func FormatISO8601Date(timestamp_second int64) string {
    tm := time.Unix(timestamp_second, 0).UTC()
    return tm.Format(ISO8601Format)
}

func ParseISO8601Date(time_string string) (time.Time, error) {
    return time.Parse(ISO8601Format, time_string)
}

func main() {
	fmt.Println("NowUTCSeconds: ", NowUTCSeconds())
	fmt.Println("NowUTCSeconds: ", FormatRFC822Date(NowUTCSeconds()))
	fmt.Println("NowUTCSeconds: ", FormatISO8601Date(NowUTCSeconds()))
}
