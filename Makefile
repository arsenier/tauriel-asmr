simavr_debug: build
	~/.platformio/packages/tool-simavr/bin/simavr -m atmega328p -f 16000000L .pio/build/nanoatmega328/firmware.elf

build:
	pio run
