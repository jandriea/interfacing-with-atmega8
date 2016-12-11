#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "rtc_handler.h"
#include "keypad_handler.h"

void keypad_init(void) {
  /* Set ROW as Output and COLUMN as input */
  DDRC |= 0x06u;
  DDRD |= 0x0Cu;

}

uint8_t keypad_GetKey(uint16_t wait_delay) {
  uint8_t key, ret_code = 'Z';

  /* Turn on all ROW */
  asm("SBI 0x15,1");
  asm("SBI 0x15,2");
  asm("SBI 0x12,2");
  asm("SBI 0x12,3");

  /* Keypad is pressed */
  while(((PIND & 0xF0) >> 4)) {
    /* Turn on first row, turn off the rest */
    asm("SBI 0x15,1");
    asm("CBI 0x15,2");
    asm("CBI 0x12,2");
    asm("CBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = '7';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '8';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '9';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = '%';
      break;
    }
    /* Turn on second row, turn off the rest */
    asm("CBI 0x15,1");
    asm("SBI 0x15,2");
    asm("CBI 0x12,2");
    asm("CBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = '4';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '5';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '6';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = 'X';
      break;
    }
    /* Turn on third row, turn off the rest */
    asm("CBI 0x15,1");
    asm("CBI 0x15,2");
    asm("SBI 0x12,2");
    asm("CBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = '1';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '2';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '3';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = '-';
      break;
    }
    /* Turn on fourth row, turn off the rest */
    asm("CBI 0x15,1");
    asm("CBI 0x15,2");
    asm("CBI 0x12,2");
    asm("SBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = 'C';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '0';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '=';
      break;
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = '+';
      break;
    }
  }


  return ret_code;
}
