#include <avr/io.h>
#include <stdint.h>
#include "led_handler.h"

void red_led_status(uint8_t stat) {
  if (stat == (uint8_t)1u) {
    asm("SBI 0x18,6");
  } else {
    asm("CBI 0x18,6");
  }
}

void blue_led_status(uint8_t stat) {
  if (stat == (uint8_t)1u) {
    asm("SBI 0x18,7");
  } else {
    asm("CBI 0x18,7");
  }
}

void yellow_led_status(uint8_t stat) {
  if (stat == (uint8_t)1u) {
    asm("SBI 0x15,3");
  } else {
    asm("CBI 0x15,3");
  }
}

void green_led_status(uint8_t stat) {
  if (stat == (uint8_t)1u) {
    asm("SBI 0x15,6");
  } else {
    asm("CBI 0x15,6");
  }
}

void red_light_district(uint16_t waitDelay) {
  if (waitDelay == (uint16_t)1u) {
    red_led_status(1u);
  }
  else if (waitDelay == (uint16_t)2u) {
    red_led_status(0u);
  }
  else if (waitDelay == (uint16_t)3u) {
    red_led_status(1u);
  }
  else if (waitDelay == (uint16_t)4u) {
    red_led_status(0u);
  }
  else {

  }
}

void blue_light_display(uint8_t st, uint8_t lm) {
  if (lm >= st) {
    blue_led_status(1);
  }
  else {
    blue_led_status(0);
  }
}
