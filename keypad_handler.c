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

  asm("SBI 0x15,1");
  asm("SBI 0x15,2");
  asm("SBI 0x12,2");
  asm("SBI 0x12,3");
  while(((PIND & 0xF0) >> 4)) {
    asm("SBI 0x15,1");
    asm("CBI 0x15,2");
    asm("CBI 0x12,2");
    asm("CBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = '7';
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '8';
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '9';
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = '%';
    }
    asm("CBI 0x15,1");
    asm("SBI 0x15,2");
    asm("CBI 0x12,2");
    asm("CBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = '4';
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '5';
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '6';
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = 'X';
    }
    asm("CBI 0x15,1");
    asm("CBI 0x15,2");
    asm("SBI 0x12,2");
    asm("CBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = '1';
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '2';
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '3';
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = '-';
    }
    asm("CBI 0x15,1");
    asm("CBI 0x15,2");
    asm("CBI 0x12,2");
    asm("SBI 0x12,3");
    while(((PIND & 0xF0) >> 4) == 0x01) {
      ret_code = 'C';
    }
    while(((PIND & 0xF0) >> 4) == 0x02) {
      ret_code = '0';
    }
    while(((PIND & 0xF0) >> 4) == 0x04) {
      ret_code = '=';
    }
    while(((PIND & 0xF0) >> 4) == 0x08) {
      ret_code = '+';
    }
  }


  return ret_code;
}
