#ifndef _STM8_H__
#define _STM8_H__
#include "stm32f10x.h"




void stm8_init(void);
void stm8_write(void);
void sum_add(void);
void uart_rec_decode(uint8_t val);
void uart_clear(void);

#endif


