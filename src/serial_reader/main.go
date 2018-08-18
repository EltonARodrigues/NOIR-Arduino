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
	n, err := s.Read(buf_message)
	if err != nil {
		log.Println(err)
		time.Sleep(TIME_WHEN_ERROR)
	}
	*b = append(*b, buf_message[:n])
	log.Println(string(buf_message[:n]))

}

func LoopWriteAndRead(s *serial.Port, t *[]byte, b *[][]byte) {
	for {
		WriteSerialToken(s, t)
		ReadSerialWithBuffer(s, b)
		time.Sleep(MSG_PER_TIME)
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
	go LoopWriteAndRead(s, &TOKEN, &BUFFER_READ)
	fmt.Scanln()
	fmt.Println(BUFFER_READ)
}

func WriteSerialToken(s *serial.Port, token *[]byte) {
	_, err := s.Write(*token)
	if err != nil {
		log.Fatal(err)
	}
}

func NormalizeBuffer(b *[]bytes.Buffer) {

}
