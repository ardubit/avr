# MCU
MCU=attiny13
F_CPU=1200000

# Fuses
# FUSE_L=0x6A
# FUSE_H=0xFF

# Compiler & Linker
CC=avr-gcc
LD=avr-ld
CS=avr-size
OBJCOPY=avr-objcopy
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I. -I/headers

# Files
TARGET=main
SRCS=main.c init.c handlers.c

# USBAsp Settings
BITCLOCK=40
	
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
	$(AVRDUDE) -p ${MCU} -B ${BITCLOCK} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb

clean:
	@printf "\n" 
	@printf " ===> Cleaning: \n"
	rm -f *.bin *.hex

# fuse:
# 	$(AVRDUDE) -p ${MCU} -c usbasp -B ${BITCLOCK} -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m
