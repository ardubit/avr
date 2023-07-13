# Serial UART
# Created: 12.07.2023 12:59:00
# Author: Aleksey M.
# Tested: YES
# ----------------------------
# Available ports:    ls /dev/tty.*
# Run:                python serial_util.py
# https://pyserial.readthedocs.io/en/latest/shortintro.html

import serial as s

baudrate = 115200
port = '/dev/tty.usbserial-1420'

# bytesize=8 
# timeout=2
# stopbits=s.STOPBITS_ONE

serial = s.Serial(port, baudrate)
print(serial.name) # Print which port was really used

serial.write(b'command') # Write string to port
serial.close()


