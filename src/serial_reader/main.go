package main

import (
	"fmt"
	"github.com/tarm/serial"
	"log"
	"os"
)

var name_usb string
var baud = 9600

func init() {
	if env := os.Getenv("PORT0"); env == "" {
		name_usb = "/dev/ttyUSB0"
	} else {
		name_usb = env
	}
}

func ReadArduino(c *serial.Config, s *serial.Port, ch chan<- string) {
	buf := make([]byte, 128)
	if n, err := s.Read(buf); err == nil {
		ch <- string(buf[:n])
	} else {
		ch <- fmt.Sprint(err)
	}
}

func main() {
	c := &serial.Config{
		Name:        name_usb,
		Baud:        baud,
	}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	response := make(chan string)
	for {
		go ReadArduino(c, s, response)
		fmt.Println(<-response)
	}
}
