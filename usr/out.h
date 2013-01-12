#ifndef _OUT_H__
#define _OUT_H__

#include "stm32f10x.h"
#include "device.h"





#define JS_MONI_ON()	(GPIOB->BSRR = GPIO_Pin_0)
#define JS_HARD_ON()	(GPIOB->BRR = GPIO_Pin_0)

#ifdef MY_BOARD

#define XB_IN_H()		(GPIOB->BSRR = GPIO_Pin_4)
#define XB_IN_L()		(GPIOB->BRR = GPIO_Pin_4)
#define LB_IN_H()		(GPIOB->BSRR = GPIO_Pin_5)
#define LB_IN_L()		(GPIOB->BRR = GPIO_Pin_5)
#define RT_IN_H()		(GPIOB->BSRR = GPIO_Pin_6)
#define RT_IN_L()		(GPIOB->BRR = GPIO_Pin_6)
#define BB_IN_H()		(GPIOB->BSRR = GPIO_Pin_7)
#define BB_IN_L()		(GPIOB->BRR = GPIO_Pin_7)
#define LE_IN_H()		(GPIOB->BSRR = GPIO_Pin_8)
#define LE_IN_L()		(GPIOB->BRR = GPIO_Pin_8)
#define RB_IN_H()		(GPIOB->BSRR = GPIO_Pin_9)
#define RB_IN_L()		(GPIOB->BRR = GPIO_Pin_9)
#define YB_IN_H()		(GPIOB->BSRR = GPIO_Pin_10)
#define YB_IN_L()		(GPIOB->BRR = GPIO_Pin_10)
#define RI_IN_H()		(GPIOB->BSRR = GPIO_Pin_11)
#define RI_IN_L()		(GPIOB->BRR = GPIO_Pin_11)
#define UP_IN_H()		(GPIOB->BSRR = GPIO_Pin_12)
#define UP_IN_L()		(GPIOB->BRR = GPIO_Pin_12)
#define DO_IN_H()		(GPIOB->BSRR = GPIO_Pin_13)
#define DO_IN_L()		(GPIOB->BRR = GPIO_Pin_13)
#define AB_IN_H()		(GPIOB->BSRR = GPIO_Pin_14)
#define AB_IN_L()		(GPIOB->BRR = GPIO_Pin_14)
#define LT_IN_H()		(GPIOB->BSRR = GPIO_Pin_15)
#define LT_IN_L()		(GPIOB->BRR = GPIO_Pin_15)

#else

#define XB_IN_H()		(GPIOB->BSRR = GPIO_Pin_4)
#define XB_IN_L()		(GPIOB->BRR = GPIO_Pin_4)
#define LB_IN_H()		(GPIOB->BSRR = GPIO_Pin_5)
#define LB_IN_L()		(GPIOB->BRR = GPIO_Pin_5)
#define RT_IN_H()		(GPIOB->BSRR = GPIO_Pin_6)
#define RT_IN_L()		(GPIOB->BRR = GPIO_Pin_6)
#define BB_IN_H()		(GPIOB->BSRR = GPIO_Pin_7)
#define BB_IN_L()		(GPIOB->BRR = GPIO_Pin_7)
#define UP_IN_H()		(GPIOB->BSRR = GPIO_Pin_8)
#define UP_IN_L()		(GPIOB->BRR = GPIO_Pin_8)
#define RB_IN_H()		(GPIOB->BSRR = GPIO_Pin_9)
#define RB_IN_L()		(GPIOB->BRR = GPIO_Pin_9)
#define YB_IN_H()		(GPIOB->BSRR = GPIO_Pin_10)
#define YB_IN_L()		(GPIOB->BRR = GPIO_Pin_10)
#define RI_IN_H()		(GPIOB->BSRR = GPIO_Pin_11)
#define RI_IN_L()		(GPIOB->BRR = GPIO_Pin_11)
#define LE_IN_H()		(GPIOB->BSRR = GPIO_Pin_12)
#define LE_IN_L()		(GPIOB->BRR = GPIO_Pin_12)
#define DO_IN_H()		(GPIOB->BSRR = GPIO_Pin_13)
#define DO_IN_L()		(GPIOB->BRR = GPIO_Pin_13)
#define AB_IN_H()		(GPIOB->BSRR = GPIO_Pin_14)
#define AB_IN_L()		(GPIOB->BRR = GPIO_Pin_14)
#define LT_IN_H()		(GPIOB->BSRR = GPIO_Pin_15)
#define LT_IN_L()		(GPIOB->BRR = GPIO_Pin_15)


#define HAND1_L()		XB_IN_L()
#define HAND1_H()		XB_IN_H()
#define HAND2_L()		BB_IN_L()
#define HAND2_H()		BB_IN_H()
#define HAND3_L()		YB_IN_L()
#define HAND3_H()		YB_IN_H()

#define FOOT1_L()		LT_IN_L()
#define FOOT1_H()		LT_IN_H()
#define FOOT2_L()		AB_IN_L()
#define FOOT2_H()		AB_IN_H()
#define FOOT3_L()		RT_IN_L()
#define FOOT3_H()		RT_IN_H()
#endif






void out_init(void);
void out_clear(void);
void out_func(void);

#endif


