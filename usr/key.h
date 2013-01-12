#ifndef _KEY_H__
#define _KEY_H__
#include "stm32f10x.h"
#include "device.h"


#ifdef MY_BOARD

#define LB_READ()		(GPIOA->IDR & 0x08)
#define LT_READ()		(GPIOA->IDR & 0x10)

#else

#define LB_READ()		(GPIOA->IDR & 0x08)
#define RB_READ()		(GPIOA->IDR & 0x04)
#define HAND1_READ()	(GPIOA->IDR & 0x40)
#define HAND2_READ()	(GPIOA->IDR & 0x01)
#define HAND3_READ()	(GPIOA->IDR & 0x02)
#define FOOT1_READ()	(GPIOA->IDR & 0x80)
#define FOOT2_READ()	(GPIOA->IDR & 0x10)
#define FOOT3_READ()	(GPIOA->IDR & 0x20)

#endif





void key_init(void);
void key_scan(void);
void key_done(void);

#endif


