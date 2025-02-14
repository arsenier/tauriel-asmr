simavr_debug: simavr_build
	simavr -m atmega328p -f 16000000L ./.pio/build/nanoatmega328/firmware.elf

simavr_build:
	pio debug

objdump: build
	avr-objdump -g -d .pio/build/uno/firmware.elf > objdump.s

build:
	pio run

clean:
	rm -rf .pio/build
