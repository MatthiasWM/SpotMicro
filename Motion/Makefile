#
# brew install arduino-cli
#

all: build/empty.ino.hex

build/empty.ino.hex: Motion.cpp Motion.h TMonitor.cpp TMonitor.h TServo.cpp TServo.h
	arduino-cli compile \
		--fqbn arduino:avr:mega:cpu=atmega2560 \
		--build-path `pwd`/build \
		--warnings all \
		--verbose \
		`pwd`/empty.ino \
		 2>&1 | sed 's|/build/sketch||'
## redirect stderr to stdout and the fix the path names in the error messages
## to point to the original files instead of their copies (just removes '/build/sketch')

.PHONY: upload
upload:
	#trap 'pkill -P $$' EXIT
	/usr/local/bin/arduino-cli upload \
		--fqbn arduino:avr:mega:cpu=atmega2560 \
		--port /dev/cu.usbmodem145101
		build/empty.ino.hex
	/opt/local/bin/socat open:/dev/cu.usbmodem145101 stdio
## don't depend on build/empty.ino.hex if run from Xcode, or we will get errors



