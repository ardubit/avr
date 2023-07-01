MCU=attiny13
F_CPU=1200000
CC=avr-gcc
CS=avr-size
OBJCOPY=avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.

# File
TARGET=main_01
SRCS=main_01.c
	
all:
	@printf "\n" 
	@printf " ===> Compiling: \n"
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	@printf "\n"
	@printf " ===> Size: \n"
	${CS} -C ${TARGET}.bin
	@printf " ===> Generating .HEX: \n"
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	
flash:
	@printf "\n" 
	@printf " ===> Flashing: \n"
	avrdude -p ${MCU} -B6 -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb

clean:
	@printf "\n" 
	@printf " ===> Cleaning: \n"
	rm -f *.bin *.hex
