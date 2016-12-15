
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"
#include "hd44780.h"
#include "led_handler.h"
#include "adc_handler.h"
#include "keypad_handler.h"
#include "rtc_handler.h"
#include "uart_handler.h"
#include "app.h"

#define RTO_TIMEOUT (100u)

/* Global Variable */
volatile uint8_t timer1_ovf_flag = (uint8_t)0u;
uint8_t serialData[30], rtc_data[25];

/* Timer 1 interrupt */
ISR(TIMER1_OVF_vect) {
  /* Reset TCNT1 value */
  TCNT1 = 0xFCF3;

  /* Set overflow flag */
  timer1_ovf_flag++;
}

int main(void){

  /* Local variable */
  uint8_t key, i, adc_delay = 0;
  uint16_t ReceivedByte, red_delay = (uint8_t)1u, keypad_delay = (uint8_t)1u, rtoCounter = (uint16_t)0u;
  rtc_t rtc;

  /* Initiate global variable */
  pressed_key = '1';
  setTemp = 0;
  lmTemp = (uint16_t)50;

  /* Set I/O direction */
  DDRB = 0xFF;
  DDRC = 0xFE;
  DDRD = 0x0F;

  /* TIMER0 init */
  /* - Prescaler = 1024 */
  /* - OVF occur every 10 ms */
  TCCR1B = (1 << CS12)|(1 << CS10);
  TIMSK = (1 << TOIE1);
  TCNT1 = 0xFCF3;

  /* LCD init without Cursor */
  lcd_init(LCD_DISPLAYMODE_ON);

  /* ADC init */
  adc_init();

  /* Keypad init */
  keypad_init();

  /* Initiate RTC */
  rtc_init();

  /* Initiate UART */
  uart_init();

  /* Enable global interrupts  */
  sei();
  rtc_getDateTime(&rtc);
  lcd_disp(&rtc, pressed_key, setTemp, lmTemp);

  for(;;) {
    blue_light_display(setTemp, lmTemp);

    /* Serial Communication */
    ReceivedByte = uart_receive();
    _delay_ms(1);
    if(ReceivedByte == '<') {
      /* Activate Yellow LED */
      yellow_led_status(ON);
      serialData[0] = ReceivedByte;
      /* Receive data and put it into buffer */
      i = (uint8_t)1u;
      while (ReceivedByte != '>') {
        ReceivedByte = uart_receive();
        _delay_ms(1);

        if(ReceivedByte != '<') {
          serialData[i] = ReceivedByte;
          i++;
        }

        rtoCounter++;
        /* if RTO timeout occur */
        if(rtoCounter > RTO_TIMEOUT) {
          break;
        }
      }
      /* Turn off Yellow LED */
      yellow_led_status(OFF);
      /*Turn on green LED*/
      green_led_status(ON);
      /* Check RTO counter */
      if(rtoCounter <= RTO_TIMEOUT) {
        serialData[i+1] = '>';
        give_me_some_reply(serialData, &rtc);
      }
      /* Turn off green LED */
      green_led_status(OFF);
      /* reset rto counter value */
      rtoCounter = (uint16_t)0u;
    }

    /* Keypad handler */
    key = keypad_GetKey(keypad_delay);
    if (key != 'Z') {
      pressed_key = key;
    }

    /* Timer overflow */
    if (timer1_ovf_flag >= (uint8_t)1u) {
      /* LED display */
      red_light_district(red_delay);

      /* update ADC every 200ms */
      if (adc_delay >= 2) {
        adc_delay = 0u;
        lmTemp = read_tmp();
      }
      else {
        adc_delay++;
      }

      if(red_delay >= (uint16_t)10u) {
        /* update every 1 sec */
        rtc_getDateTime(&rtc);
        red_delay = (uint16_t)1u;
        lcd_disp(&rtc, pressed_key, setTemp, lmTemp);
      }
      else {
        red_delay++;
      }
      /* Clear overflow flag */
      timer1_ovf_flag = (uint8_t)0u;
    }
  }

  return 0;
}
