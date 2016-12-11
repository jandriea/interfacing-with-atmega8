#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "main.h"
#include "rtc_handler.h"
#include "uart_handler.h"
#include "keypad_handler.h"

#define BAUD (9600)
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define VALID ((uint8_t)1u)
#define NOT_VALID ((uint8_t)0u)

uint8_t isDateValid(uint8_t data[30]);

void uart_init(void) {
  UBRRH = (BAUDRATE>>8);
  UBRRL = BAUDRATE;
  UCSRB|= (1<<TXEN)|(1<<RXEN);
  UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void uart_transmit(uint8_t data) {
  while (!( UCSRA & (1<<UDRE)));
  UDR = data;
}

uint8_t uart_receive(void) {
  while(!(UCSRA) & (1<<RXC));
  return UDR;
}

void give_me_some_reply(uint8_t data[30], rtc_t *rtc) {
  uint8_t buff[25];
  rtc_t new_rtc;

  uart_transmit('<');
  if (data[1] == 'G' && data[2] == 'T') {
    uart_transmit(intToChar(setTemp/10));
    uart_transmit(intToChar(setTemp%10));
    uart_transmit(',');
    uart_transmit(intToChar(lmTemp/10));
    uart_transmit(intToChar(lmTemp%10));
  }
  else if (data[1] == 'S' && data[2] == 'T') {
    uint8_t stData1 = charToInt(data[3]), stData2 = charToInt(data[4]);
    if (stData1 != 10 && stData2 != 10) {
      uart_transmit('O');
      uart_transmit('K');
      setTemp = stData1 * 10 + stData2;
    }
    else {
      for (uint8_t i = 1; data[i] != '>'; i++) {
        uart_transmit(data[i]);
      }
    }
  }
  else if (data[1] == 'G' && data[2] == 'K') {
    uart_transmit(pressed_key);
  }
  else if (data[1] == 'S' && data[2] == 'K') {
    if (((data[3]) == '1') | ((data[3]) == '2') | ((data[3]) == '3') | ((data[3]) == '4') | ((data[3]) == '5') | ((data[3]) == '6') | ((data[3]) == '7') | ((data[3]) == '8') | ((data[3]) == '9') | ((data[3]) == '%') | ((data[3]) == '-') | ((data[3]) == 'C') | ((data[3]) == '+') | ((data[3]) == '0') | ((data[3]) == '=') | ((data[3]) == 'X') ) {
      uart_transmit('O');
      uart_transmit('K');
      pressed_key = data[3];
    }
    else {
      for (uint8_t i = 1; data[i] != '>'; i++) {
        uart_transmit(data[i]);
      }
    }

  }
  else if (data[1] == 'S' && data[2] == 'R') {
    uint8_t rc = isDateValid(data);
    if (rc == VALID) {
      rtc_convertFromArray(rtc, data);
      rtc_setDateTime(rtc);
      uart_transmit('O');
      uart_transmit('K');
    }
    else {
      for(uint8_t i = 1; data[i] != '>'; i++) {
        uart_transmit(data[i]);
      }
    }

  }
  else if (data[1] == 'G' && data[2] == 'R') {
    rtc_converToArray(rtc, buff);
    for (uint8_t i = 0; i < 23; i++) {
      uart_transmit(buff[i]);
    }
  }
  else {
    for(uint8_t i = 1; data[i] != '>'; i++) {
      uart_transmit(data[i]);
    }
  }
  uart_transmit('>');
  uart_transmit('\r');
  uart_transmit('\n');
}

uint8_t isDateValid(uint8_t data[30]) {
  uint8_t ret = 0, dow_valid = NOT_VALID, date, date_valid = VALID, month, month_valid= VALID, year, year_valid= VALID, leap_year, hour, hour_valid = VALID, min, min_valid = VALID, sec, sec_valid = VALID;

  /* Check dow */
  if((data[4] == 'S') && (data[5] == 'U') && (data[6] == 'N')) {
    dow_valid = VALID;
  }
  else if((data[4] == 'M') && (data[5] == 'O') && (data[6] == 'N')) {
    dow_valid = VALID;
  }
  else if((data[4] == 'T') && (data[5] == 'U') && (data[6] == 'E')) {
    dow_valid = VALID;
  }
  else if((data[4] == 'W') && (data[5] == 'E') && (data[6] == 'D')) {
    dow_valid = VALID;
  }
  else if((data[4] == 'T') && (data[5] == 'H') && (data[6] == 'U')) {
    dow_valid = VALID;
  }
  else if((data[4] == 'F') && (data[5] == 'R') && (data[6] == 'I')) {
    dow_valid = VALID;
  }
  else if((data[4] == 'S') && (data[5] == 'A') && (data[6] == 'T')) {
    dow_valid = VALID;
  }
  else {
    dow_valid = NOT_VALID;
  }

  /* Check year */
  if (charToInt(data[14]) == 10 | charToInt(data[15]) == 10 | charToInt(data[16]) == 10 | charToInt(data[17]) == 10) {
    year_valid = NOT_VALID;
  }
  else {
    year_valid = VALID;
    year = charToInt(data[14]) * 1000 + charToInt(data[15]) * 100 + charToInt(data[16]) * 10 + charToInt(data[17]);
    if ((year % 4 == 0) && ((year % 100 == 0) && (year % 400 == 0))) {
      leap_year = VALID;
    }
    else {
      leap_year = NOT_VALID;
    }
    month = charToInt(data[11]) * 10 + charToInt(data[12]);
    if (month > 12) {
      month_valid = NOT_VALID;
    }
    else {
      month_valid = VALID;
      date = charToInt(data[8]) * 10 + charToInt(data[9]);
      if (date <= 31) {
        date_valid = VALID;

        if ((date == 31) && ((month == 2) | (month == 4) | (month == 6) | (month == 9) | (month == 11))) {
          date_valid = NOT_VALID;
        }

        if ((date == 30) && (month == 2) && (date_valid == VALID)) {
          date_valid = NOT_VALID;
        }

        if ((date == 29) && (month == 2) && (leap_year != VALID) && (date_valid == VALID)) {
          date_valid == NOT_VALID;
        }
      }
      else {
        date_valid = NOT_VALID;
      }
    }
  }


  /* Check hour */
  if ((charToInt(data[19]) == 10) | (charToInt(data[20]) == 10)) {
    hour_valid = NOT_VALID;
  }
  else {
    hour = charToInt(data[19]) * 10 + charToInt(data[20]);
    if (hour <= 59) {
      hour_valid = VALID;
    }
    else {
      hour_valid = NOT_VALID;
    }
  }

  /* Check min */
  if ((charToInt(data[22]) == 10) | (charToInt(data[23]) == 10)) {
    min_valid = NOT_VALID;
  }
  else {
    min = charToInt(data[22]) * 10 + charToInt(data[23]);
    if (min <= 59) {
      min_valid = VALID;
    }
    else {
      min_valid = NOT_VALID;
    }
  }

  /* Check sec */
  if ((charToInt(data[25]) == 10) | (charToInt(data[26]) == 10)) {
    sec_valid = NOT_VALID;
  }
  else {
    sec = charToInt(data[25]) * 10 + charToInt(data[26]);
    if (sec <= 59) {
      sec_valid = VALID;
    }
    else {
      sec_valid = NOT_VALID;
    }
  }

  ret = dow_valid & date_valid & month_valid & year_valid & hour_valid & min_valid & sec_valid;

  return ret;
}
