#include <avr/io.h>
#include "hd44780.h"
#include "rtc_handler.h"

void lcd_disp(rtc_t *rtc, uint8_t key, uint8_t st_tmp, uint8_t lm_tmp) {
  uint8_t buff_key, buff_st="", buff_lm="";
  uint8_t buff[25];
  rtc_converToArray(rtc, buff);
  /* Clear Screen */
  lcd_clrscr();

  /* Go to home */
  lcd_home();
  //lcd_puts(buff_dow);
  lcd_putc(buff[0]);
  lcd_putc(buff[1]);
  lcd_putc(buff[2]);

  lcd_goto(0x04);
  /* Set date */
  lcd_putc(buff[4]);
  lcd_putc(buff[5]);
  lcd_putc(buff[6]);
  /* Set month */
  lcd_putc(buff[7]);
  lcd_putc(buff[8]);
  lcd_putc(buff[9]);
  /* Set year */
  lcd_putc(buff[10]);
  lcd_putc(buff[11]);
  lcd_putc(buff[12]);
  lcd_putc(buff[13]);

  lcd_goto(0x0F);

  /* Last pressed key */
  lcd_putc(key);

  lcd_goto(0x40);
  /* Hour */
  lcd_putc(buff[15]);
  lcd_putc(buff[16]);
  lcd_putc(buff[17]);
  /* Min */
  lcd_putc(buff[18]);
  lcd_putc(buff[19]);
  lcd_putc(buff[20]);
  /* Sec */
  lcd_putc(buff[21]);
  lcd_putc(buff[22]);

  /* Limit temperature */
  lcd_goto(0x4A);
  if(st_tmp < 10) {
    lcd_putc('0');
  }
  itoa(st_tmp, buff_st, 10);
  lcd_puts(buff_st);

  /* LM35 temperature */
  lcd_goto(0x4E);
  if(lm_tmp < 10) {
    lcd_putc('0');
  }
  itoa(lm_tmp, buff_lm, 10);
  lcd_puts(buff_lm);
}
