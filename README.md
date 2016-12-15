# interfacing-with-atmega8
ATMEGA8 with UART, Keypad, RTC, LM35 and LCD and LEDs

##UART
Baudrate = 9600, parity = none, data = 8 bit
Serial command:
- <GT> Get temperature limit, LM35 temperature
- <STxx> set temperature limit (00 - 99)
- <GK> Get last pressed keypad
- <SKx> Set last pressed keypad display
- <GR> Get RTC value DOW,dd-MM-yyyy,hh:mm:ss
- <SR,DOW,dd-MM-yyyy,hh:mm:ss> Set RTC value

##Keypad
I am using 4x4 keypad from proteus, calculator like keypad

##RTC
DS1307 with I2C communication (using Peter Fluery code)

##LM35
Read Temperature by dividing ADC value with 4, Value will be updated every 200ms

##LCD
HD44780 using 6 pin configuration, Write only (Using SA Development code)

##LED
- Red LED -> Periodic blink (200ms ON, 800ms OFF)
- Blue LED -> Turn on when lm35 temp > limit temp
- Yellow LED -> Rx indicator
- Green LED -> Tx indicator

There is also proteus simulation in this repo, (Proteus 7.8 version)

#How to use
- Make sure you have avr-gcc installed (I am using 4.9.2 version)
- Run "make clean" in your terminal
- Run "make" in your terminal

#How to simulate with proteus
- load the file
- insert your hex files to mcu
- set RSTDSBLE fuse programmed
- run simulation
