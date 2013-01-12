#ifndef _KEY_H__
#define _KEY_H__
#include "stm32f10x.h"



#define KEY1_READ()		(GPIOC->IDR & GPIO_Pin_13)
#define KEY2_READ()		(GPIOC->IDR & GPIO_Pin_14)
#define KEY3_READ()		(GPIOC->IDR & GPIO_Pin_15)






void key_init(void);
void key_scan(void);
void key_done(void);

#endif


