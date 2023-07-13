# MCU
MCU=attiny24
# default fuses
FUSE_L=0x62
FUSE_H=0xDF
FUSE_E=0xFF
F_CPU=1000000

# Compiler & Linker
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.

# Files
TARGET=main
SRCS = main.c

# USBAsp Settings
BITCLOCK=10

all:
	@printf "\n" 
	@printf " ===> Compile HEX: \n"
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

flash:
	@printf "\n" 
	@printf " ===> Flash: \n"
	${AVRDUDE} -p ${MCU} -c usbasp -B${BITCLOCK} -U flash:w:${TARGET}.hex:i -F -P usb

rfuse:
	@printf "\n" 
	@printf " ===> Read Fuses: \n"
	$(AVRDUDE) -p ${MCU} -c usbasp -B${BITCLOCK} -U hfuse:r:-:h -U lfuse:r:-:h -U efuse:r:-:h

wfuse:
	@printf "\n" 
	@printf " ===> Write Fuses: \n"
	$(AVRDUDE) -p ${MCU} -c usbasp -B${BITCLOCK} -U lfuse:w:${FUSE_L}:m -U hfuse:w:${FUSE_H}:m -U efuse:w:${FUSE_E}:m

clean:
	rm -f *.c~ *.o *.elf *.hex
