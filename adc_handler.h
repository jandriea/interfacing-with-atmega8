#ifndef _ADC_HANDLER_H
#define _ADC_HANDLER_H

void adc_init(void);
uint16_t adc_read(uint8_t ch);
uint16_t read_tmp(void);

#endif
