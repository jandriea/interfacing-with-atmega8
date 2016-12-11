#include <avr/io.h>
#include <stdint.h>
#include "rtc_handler.h"
#include "i2cmaster.h"

#define DS1307_ADDRESS_READ   0xD1
#define DS1307_ADDRESS_WRITE  0xD0
#define SECONDS_ADDRESS       0x00
#define MINUTES_ADDRESS       0x01
#define HOURS_ADDRESS         0x02
#define DOW_ADDRESS           0x03
#define DATE_ADDRESS          0x04
#define MONTH_ADDRESS         0x05
#define YEAR_ADDRESS          0x06
#define CONTROL_ADDRESS       0x07

uint8_t intToChar(uint8_t num);
uint8_t charToInt(uint8_t num);

void rtc_init(void) {
  /* I2C connection init */
  i2c_init();

  /* Start communicating */
  i2c_start(DS1307_ADDRESS_WRITE);

  /* Set memory address */
  i2c_write(CONTROL_ADDRESS);

  /* Write data to memory address */
  i2c_write(0x00);

  /* Stop connection */
  i2c_stop();
}

void rtc_setDateTime(rtc_t *rtc) {
  /* Start communicating */
  i2c_start(DS1307_ADDRESS_WRITE);

  /* Set memory address */
  i2c_write(SECONDS_ADDRESS);

  /* Write data to memory address */
  i2c_write(rtc->sec);
  i2c_write(rtc->min);
  i2c_write(rtc->hr);
  i2c_write(rtc->dow);
  i2c_write(rtc->date);
  i2c_write(rtc->month);
  i2c_write(rtc->year);

  /* Stop connection */
  i2c_stop();
}

void rtc_getDateTime(rtc_t *rtc) {
  /* Start communicating */
  i2c_start(DS1307_ADDRESS_WRITE);
  /* Set memory address */
  i2c_write(SECONDS_ADDRESS);

  /* Start read */
  i2c_rep_start(DS1307_ADDRESS_READ);
  rtc->sec = i2c_readAck();
  rtc->min = i2c_readAck();
  rtc->hr = i2c_readAck();
  rtc->dow = i2c_readAck();
  rtc->date = i2c_readAck();
  rtc->month = i2c_readAck();
  rtc->year = i2c_readNak();

  i2c_stop();
}

void rtc_converToArray(rtc_t *rtc, uint8_t arr[25]) {
  uint8_t dow, min, hour, sec, date, month, year;

  dow = rtc->dow;
  date = rtc->date;
  month = rtc->month;
  year = rtc->year;
  hour = rtc->hr;
  min = rtc->min;
  sec = rtc->sec;

  switch (dow) {
    case 1: {
      arr[0] = 'S';
      arr[1] = 'U';
      arr[2] = 'N';
    } break;
    case 2: {
      arr[0] = 'M';
      arr[1] = 'O';
      arr[2] = 'N';
    } break;
    case 3: {
      arr[0] = 'T';
      arr[1] = 'U';
      arr[2] = 'E';
    } break;
    case 4: {
      arr[0] = 'W';
      arr[1] = 'E';
      arr[2] = 'D';
    } break;
    case 5: {
      arr[0] = 'T';
      arr[1] = 'H';
      arr[2] = 'U';
    } break;
    case 6: {
      arr[0] = 'F';
      arr[1] = 'R';
      arr[2] = 'I';
    } break;
    case 7: {
      arr[0] = 'S';
      arr[1] = 'A';
      arr[2] = 'T';
    } break;
    default: {
      arr[0] = 'E';
      arr[1] = 'R';
      arr[2] = 'R';
    } break;
  }

  arr[3] = ',';
  arr[4] = intToChar((date & 0x3F) >> 4);
  arr[5] = intToChar(date & 0x0F);
  arr[6] = '-';
  arr[7] = intToChar((month & 0x1F) >> 4);
  arr[8] = intToChar(month & 0x0F);
  arr[9] = '-';
  arr[10] = '2';
  arr[11] = '0';
  arr[12] = intToChar((year & 0xFF) >> 4);
  arr[13] = intToChar(year & 0x0F);
  arr[14] = ',';
  arr[15] = intToChar((hour & 0x3F) >> 4);
  arr[16] = intToChar(hour & 0x0F);
  arr[17] = ':';
  arr[18] = intToChar((min & 0x7F) >> 4);
  arr[19] = intToChar(min & 0x0F);
  arr[20] = ':';
  arr[21] = intToChar((sec & 0x7F) >> 4);
  arr[22] = intToChar(sec & 0x0F);
}

void rtc_convertFromArray(rtc_t *rtc, uint8_t arr[30]) {
  if((arr[4] == 'S') && (arr[5] == 'U')) {
    rtc->dow = 0x01;
  }
  else if(arr[0] == 'M') {
    rtc->dow = 0x02;
  }
  else if((arr[4] == 'T') && (arr[5] == 'U')) {
    rtc->dow = 0x03;
  }
  else if(arr[4] == 'W') {
    rtc->dow = 0x04;
  }
  else if((arr[4] == 'T') && (arr[5] == 'H')) {
    rtc->dow = 0x05;
  }
  else if(arr[4] == 'F') {
    rtc->dow = 0x06;
  }
  else if((arr[4] == 'S') && (arr[5] == 'A')) {
    rtc->dow = 0x07;
  }
  else {

  }

  rtc->date = (charToInt(arr[8]) & 0x0F) << 4 | (charToInt(arr[9]) & 0x0F);
  rtc->month = (charToInt(arr[11]) & 0x0F) << 4 | (charToInt(arr[12]) & 0x0F);
  rtc->year = (charToInt(arr[16]) & 0x0F) << 4 | (charToInt(arr[17]) & 0x0F);
  rtc->hr = (charToInt(arr[19]) & 0x0F) << 4 | (charToInt(arr[20]) & 0x0F);
  rtc->min = (charToInt(arr[22]) & 0x0F) << 4 | (charToInt(arr[23]) & 0x0F);
  rtc->sec = (charToInt(arr[25]) & 0x0F) << 4 | (charToInt(arr[26]) & 0x0F);
}

uint8_t intToChar(uint8_t num) {
  uint8_t ret;
  switch (num) {
    case 1: ret = '1'; break;
    case 2: ret = '2'; break;
    case 3: ret = '3'; break;
    case 4: ret = '4'; break;
    case 5: ret = '5'; break;
    case 6: ret = '6'; break;
    case 7: ret = '7'; break;
    case 8: ret = '8'; break;
    case 9: ret = '9'; break;
    case 0: ret = '0'; break;
    default: break;
  }

  return ret;
}

uint8_t charToInt(uint8_t num) {
  uint8_t ret;
  switch (num) {
    case '1': ret = 1; break;
    case '2': ret = 2; break;
    case '3': ret = 3; break;
    case '4': ret = 4; break;
    case '5': ret = 5; break;
    case '6': ret = 6; break;
    case '7': ret = 7; break;
    case '8': ret = 8; break;
    case '9': ret = 9; break;
    case '0': ret = 0; break;
    default: ret = 10; break;
  }

  return ret;
}
