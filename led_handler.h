#ifndef LED_HANDLER_H
#define LED_HANDLER_H

void red_led_status(uint8_t stat);
void blue_led_status(uint8_t stat);
void yellow_led_status(uint8_t stat);
void green_led_status(uint8_t stat);
void red_light_district(uint16_t waitDelay);
void blue_light_display(uint8_t st, uint8_t lm);

#endif
