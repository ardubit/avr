#
MCU=attiny13
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes

# Files
FILE_NAME = main
TARGET=${FILE_NAME}
SRCS = ${FILE_NAME}.c

#-------------------
all: hex
#-------------------
hex: out 
	$(OBJCOPY) -R .eeprom -O ihex ${TARGET}.out ${TARGET}.hex 
out: o 
	$(CC) $(CFLAGS) -o ${TARGET}.out -Wl,-Map,${TARGET}.map ${TARGET}.o 
o:
	$(CC) $(CFLAGS) -Os -c ${SRCS}
clean:
	rm -f *.o *.map *.out *.hex