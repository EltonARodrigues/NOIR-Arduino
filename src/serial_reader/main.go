  package main

import (
	"bytes"
	"fmt"
	"github.com/tarm/serial"
	"log"
	"os"
	"time"
)

var SERIAL_PATH string
var BAUD = 9600
var BUFFER_READ bytes.Buffer

const TIME_WHEN_ERROR = 5 * time.Second
const MAX_LEN_MESSAGE = 60

func init() {
	if env := os.Getenv("PORT0"); env == "" {
		SERIAL_PATH = "/dev/ttyUSB0"
	} else {
		SERIAL_PATH = env
	}
}

func ReadSerialWithBuffer(c *serial.Config, s *serial.Port, b *bytes.Buffer) {
	buf_message := make([]byte, MAX_LEN_MESSAGE)
	for {
		n, err := s.Read(buf_message)
		if err != nil {
			log.Println(err)
			time.Sleep(TIME_WHEN_ERROR)
		}
		b
		b.Write(buf_message[:n])
		// b.WriteRune(10) // \n
		log.Println(string(buf_message[:n]))
	}
}

func main() {
	c := &serial.Config{
		Name: SERIAL_PATH,
		Baud: BAUD,
	}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}
	go ReadSerialWithBuffer(c, s, &BUFFER_READ)
	go PrintBuffer(&BUFFER_READ)
	fmt.Println(BUFFER.String())
}


func PrintBuffer(b *bytes.Buffer){
	for{
		if len(b) != 0{

		}
	}
}
