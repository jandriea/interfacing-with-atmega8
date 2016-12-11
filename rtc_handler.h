#ifndef RTC_HANDLER_H
#define RTC_HANDLER_H

typedef struct {
  uint8_t sec;
  uint8_t min;
  uint8_t hr;
  uint8_t dow;
  uint8_t date;
  uint8_t month;
  uint8_t year;
}rtc_t;

void rtc_init(void);
void rtc_setDateTime(rtc_t *rtc);
void rtc_getDateTime(rtc_t *rtc);
void rtc_converToArray(rtc_t *rtc, uint8_t arr[25]);
void rtc_convertFromArray(rtc_t *rtc, uint8_t arr[30]);
uint8_t intToChar(uint8_t num);
uint8_t charToInt(uint8_t num);

#endif
