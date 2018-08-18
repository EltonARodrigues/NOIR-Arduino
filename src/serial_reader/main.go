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
var BUFFER_READ [][]byte
var TOKEN = []byte("1")

const TIME_WHEN_ERROR = 5 * time.Second
const MAX_LEN_MESSAGE = 60
const MSG_PER_TIME = time.Second

func init() {
	if env := os.Getenv("ARDUINO"); env == "" {
		SERIAL_PATH = "/dev/ttyUSB0"
	} else {
		SERIAL_PATH = env
	}
}

func ReadSerialWithBuffer(s *serial.Port, b *[][]byte) {
	buf_message := make([]byte, MAX_LEN_MESSAGE)
	for {
		n, err := s.Read(buf_message)
		if err != nil {
			log.Println(err)
			time.Sleep(TIME_WHEN_ERROR)
		}
		*b = append(*b, buf_message[:n])
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
	go WriteSerialToken(s, &TOKEN)
	go ReadSerialWithBuffer(s, &BUFFER_READ)
	fmt.Scanln()
	fmt.Println(BUFFER_READ)
}

// This is need be in a fuction with ReadSerialWithBuffer
func WriteSerialToken(s *serial.Port, token *[]byte) {
	for {
		_, err := s.Write(*token)
		if err != nil {
			log.Fatal(err)
		}
		time.Sleep(MSG_PER_TIME)
	}
}

func NormalizeBuffer(b *[]bytes.Buffer) {

}
