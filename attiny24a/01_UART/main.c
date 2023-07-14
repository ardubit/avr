/*
 * Universal Asynchronous Receiver/Transmitter. USI UART
 * Created: 10.07.2023
 * Author: Aleksey M.
 * MCU ATtiny24a
 * Tested:
 */

// MCU Frequency - 1,0 MHz, page 36
#define F_CPU 1000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/iotn24.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define LED PA0
#define USCK PA4
#define TXDO PA5
#define RXDI PA6


// UART
#define BAUDRATE 9600
#define CYCLES_PERBIT (F_CPU / BAUDRATE)

#if (CYCLES_PERBIT > 255)
#define DIVIDER 8
#define PRESCALE 2
#else
#define DIVIDER 1
#define PRESCALE 1
#endif
#define BIT_TICKS (CYCLES_PERBIT / DIVIDER)

void initGPIO() {
  //
  DDRA |= (1 << LED);
}

void init_USIUART() {
  //
  DDRA |= (1 << TXDO);  // TXDO
  PORTA |= (1 << TXDO); // idle (HI)

  // Debug out. Clock generation when implementing master devices. USITC: Toggle Clock Port Pin, page 131
  DDRA |= (1 << USCK);

  // USI Mode. The mode that should be used for UART communica- tion is the SPI mode [doc4300, page 4]. The USI Three-wire mode is compliant to the Serial Peripheral Interface (SPI) [datasheet, page 123]
  USICR |= (1 << USIWM0);

}

// UART sends the bits LSB first, but the USI assumes that the MSB is first
unsigned char revByte(unsigned char x) {
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}

void sendChar_USIUART(unsigned char data) {
  //
}

char recvChar_USIUART() {
  //
  return USIBR;
}

// Numbers to strings

// Strings to numbers

ISR(USI_OVF_vect) {
  // When 8 bits have been shifted USI overflow interrupt occurs
}

ISR(PCINT0_vect) {
  // The pin change interrupt can thus be used to detect the start-bit of a UART frame, (PCINT6)
}

int main(int argc, char const *argv[]) {
  initGPIO();
  init_USIUART();

  const char *str = "ATtiny24 USI UART message";
  const uint8_t *msg = (const uint8_t *)"Test";

  while (1) {
    // Transmit a test message
    for (int i = 0; str[i] != '\0'; i++) {
      sendChar_USIUART(str[i]);
    }
    sendChar_USIUART('\n');

    _delay_ms(1000); // Delay for 1 second
  }

  return 0;
}

/*
# Comments:
===================================
## What does the program do?
It sends a string via UART to the PC application and listens to a command to
turn on the LED connected to a specific port.

The ATtiny24 microcontroller does not have a dedicated USART (Universal
Synchronous and Asynchronous Receiver and Transmitter) module like some other
microcontrollers. Instead, it uses the USI (Universal Serial Interface) for
serial communication. Full-duplex means transmission can occur in both directions simultaneously.

Circuit connections:
1  VCC   +3,3-5V
2  PB0 - *
3  PB1 - *
5  PB2 - *
4  PB3 - Reset (input)

14 GND
13 PA0 - LED - 2,2k - GND
12 PA1 - *
11 PA2 - *
10 PA3 - *
9  PA4 - USCK, SCL
8  PA5 - MISO, DO (TX)
7  PA6 - MOSI, DI (RX)
6  PA7 - *

## Notes:
-----------------------------------
UART package
- idle (HI)
- Start bit
- 5 to 9 data bits
- Acknowledgment bit (optionaly)
- 1 or 2 Stop bits (HI)

TX output form looks like (which is the input form for RX):
10 bit frame = start + 8 bit data + stop
TX ->
RX <-

idle  start            data                stop  idle
_____      ___ ___ ___ ___ ___ ___ ___ ___ _____ ______
     |____|_0_|_1_|_2_|_3_|_4_|_5_|_6_|_7_|     |


# Baud
The term 'baud' represents the number of signal or symbol changes per second. 1
baud = 1 bit/sec

# Communication
Device A     Device B
      TX --> RX
      RX <-- TX
     GND --- GND


# Classic UART Hardware (USI is similar)
==============================================================================
## Transmitter
Software layer:
1. [Write] to Data register 8 bit 
2. Send to FIFO buffer 16 bit and then to hardware shift register
3. Push to Hardware shift register:
| stop |7|6|5|4|3|2|1|0| start | --> directly connected to the TX pin

## Receiver
---------------
1. Pick data from shift register:
| stop |7|6|5|4|3|2|1|0| start | --> directly connected to the RX pin

2. Fill 16 bits FIFO buffer. 
However with 8-bit of data and 4 bits for Errors (plus start/stop, ack bits)
 - an overrun error bit(OE)
 - a break error bit(BE)
 - a frame error bit(FE) or baud error
 - and a parity error bit(PE)

FIFO buffer:
| OE | BE | FE | PE | data |

3. [Read] from Data register 8 bit


## Complete diagram
==============================================================================
TX                                            RX
| stop |7|6|5|4|3|2|1|0| start | -----------> | stop |7|6|5|4|3|2|1|0| start |
| FIFO Hardware or software    |              | OE | BE | FE | PE | 7 data 0 |
| [write] DATA Register        |              | [Read] DATA register         |

## Observations
==============================================================================
- If you change the cpu/bus clock frequency without changing the baud rate register, the UART will operate at an incorrect baud rate. 

- When you set UART speed for the transmitter or receiver it means you define a certain frequency (set prescaler) for the UART hardware of your device to match with another connected unit, because they could have different cpu/bus clocks.

- The baud rates in the transmitter and receiver must match within 5% for the channel to operate properly. The error for this example is 0.002%.


# Open PC UART (Mac, Unix)
--------------------------
ls /dev/tty.*
screen /dev/tty.usbserial-1410 9600


# Conclusion
===================================
Run this code and find what behavior is expected and what we got.
*/
