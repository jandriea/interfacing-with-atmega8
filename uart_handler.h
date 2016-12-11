#ifndef UART_HANDLER_H
#define UART_HANDLER_H

void uart_init(void);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);
void give_me_some_reply(uint8_t data[30], rtc_t *rtc);

#endif
