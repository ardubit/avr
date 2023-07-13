/*
 * Universal Asynchronous Receiver/Transmitter. USI UART
 * Created: 10.07.2023
 * Author: Aleksey M.
 * MCU ATtiny24a
 * Tested:
 */

// MCU Frequency - 1,0 MHz, page 36
#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/iotn24.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define LED PA0
#define TXDO PA5
#define RXDI PA6

// UART
#define BAUDRATE 9600
#define CYCLES_PERBIT ( F_CPU / BAUDRATE )

void initGPIO() { DDRB |= (1 << LED); }

/*
  The ATtiny24 microcontroller does not have a dedicated USART (Universal
  Synchronous and Asynchronous Receiver and Transmitter) module like some other
  microcontrollers. Instead, it uses the USI (Universal Serial Interface) for
  serial communication.
*/

void init_USIUART() {
 //
}

void sendChar_USIUART(unsigned char data) {
//
}

char recvChar_USIUART() {
  //
}

int main(int argc, char const *argv[]) {
  initGPIO();
  init_USIUART();

  const char *str = "ATtiny24 USI UART message";

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
- 1 or 2 Stop bits

idle  start            data                stop  idle
_____      ___ ___ ___ ___ ___ ___ ___ ___ _____ ______
     |____|_0_|_1_|_2_|_3_|_4_|_5_|_6_|_7_|     |


# Open PC UART (Mac, Unix)
--------------------------
ls /dev/tty.*
screen /dev/tty.usbserial-1410 9600

# Conclusion
===================================
Run this code and find what behavior is expected and what we got.
*/
