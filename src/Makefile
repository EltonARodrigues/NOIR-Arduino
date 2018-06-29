current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

ARDUINO_DIR  = /usr/share/arduino
#ARDUINO_LIBS = Ethernet Ethernet/utility SPI
BOARD_TAG    = uno #nano328 #uno
ARDUINO_PORT = /dev/ttyACM0 #/dev/ttyACM0
ARDUINO_SKETCHBOOK := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

include /usr/share/arduino/Arduino.mk
