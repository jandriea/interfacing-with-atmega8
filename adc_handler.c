#include <avr/io.h>
#include <stdint.h>
#include "adc_handler.h"

void adc_init(void) {
  /* Internal 2.56V */
  ADMUX = (1<<REFS0)|(1<<REFS1);

  /* Enable ADC read with 128 prescaler */
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch) {
  /* Use selected channel to convert */
  /* AND input value with '7' to make sure uC uses correct channel */
  ch &= 0x00000111;
  ADMUX = (ADMUX & 0xF8)|ch;

  /* Start single conversion */
  ADCSRA |= (1<<ADSC);

  /* Wait the conversion complete (ADSC = 0)*/
  while (ADCSRA & (1<<ADSC)) {
    ;
  }

  return (ADC);
}

/* Read temperature in Celcius Degree */
uint16_t read_tmp(void){
  return (adc_read(0) / 2) ;
}
