#include "out.h"

extern uint8_t out_flag1, out_flag2;
extern uint8_t out_state1, out_state2;
extern uint8_t key_enable;
extern uint8_t out_hand1, out_hand2, out_hand3;
extern uint8_t out_foot1, out_foot2, out_foot3;


void out_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	JS_HARD_ON();
	out_clear();
}


void out_clear(void)
{
	XB_IN_H();
	LB_IN_H();
	RT_IN_H();
	BB_IN_H();
	LE_IN_H();
	RB_IN_H();
	YB_IN_H();
	RI_IN_H();
	UP_IN_H();
	DO_IN_H();
	AB_IN_H();
	LT_IN_H();
}


//出招，1ms进入一次
void out_func(void)
{
	static uint16_t i = 0;
	static uint8_t step = 0;
	
	if (out_flag1 == 1)
	{
		//发招
		if (out_state1 == 2)
		{
			switch (step)
			{
			case 0:
				
				DO_IN_L();
				LE_IN_L();
				i ++;
				
				if (i == 20)
				{
					i = 0;
					step = 1;
					LE_IN_H();
				}
				break;
				
			case 1:
				
				DO_IN_L();
				RI_IN_L();
				i ++;
				
				if (i == 20)
				{
					i = 0;
					step = 2;
					RI_IN_H();
				}
				break;
				
			case 2:
				
				DO_IN_L();
				LE_IN_L();
				i ++;
				
				if (i == 20)
				{
					i = 0;
					step = 3;
					LE_IN_H();
					DO_IN_H();
				}
				break;
				
			case 3:
				
				RI_IN_L();
				HAND2_L();
				i ++;
				
				if (i == 20)
				{
					//最终释放，不要修改
					i = 0;
					step = 0;
					out_flag1 = 0;
					out_clear();
					JS_HARD_ON();
					key_enable = 1;
				}
				break;
			}		
		}
	}
	
	
	if (out_flag2 == 1)
	{
		//发招
		if (out_state2 == 1)
		{
			switch (step)
			{
			case 0:
				
				DO_IN_L();
				LE_IN_L();
				i ++;
				
				if (i == 20)
				{
					i = 0;
					step = 1;
					LE_IN_H();
				}
				break;
				
			case 1:
				
				DO_IN_L();
				RI_IN_L();
				i ++;
				
				if (i == 20)
				{
					i = 0;
					step = 2;
					RI_IN_H();
				}
				break;
				
			case 2:
				
				DO_IN_L();
				LE_IN_L();
				i ++;
				
				if (i == 20)
				{
					i = 0;
					step = 3;
					LE_IN_H();
					DO_IN_H();
				}
				break;
				
			case 3:
				
				RI_IN_L();
				HAND1_L();
				i ++;
				
				if (i == 20)
				{
					//最终释放，不要修改
					i = 0;
					step = 0;
					out_flag2 = 0;
					out_clear();
					JS_HARD_ON();
					key_enable = 1;
				}
				break;
			}			
		}	
	}
	
	
	//黄拳宏
	if (out_hand1 == 1)
	{
		switch (step)
		{
		case 0:
			
			step = 1;
			break;
			
		case 1:
			
			i ++;
			
			if (i == 20)
			{
				//最终释放，不要修改
				i = 0;
				step = 0;
				out_hand1 = 0;
				out_clear();
				JS_HARD_ON();
				key_enable = 1;
			}
			break;
		}
	}
	
	//蓝拳宏
	if (out_hand2 == 1)
	{
		switch (step)
		{
		case 0:
			
			step = 1;
			break;
			
		case 1:
			
			i ++;
			
			if (i == 20)
			{
				//最终释放，不要修改
				i = 0;
				step = 0;
				out_hand2 = 0;
				out_clear();
				JS_HARD_ON();
				key_enable = 1;
			}
			break;
		}
	}
	
	
	//红拳宏
	if (out_hand3 == 1)
	{
		switch (step)
		{
		case 0:
			
			step = 1;
			break;
			
		case 1:
			
			i ++;
			
			if (i == 20)
			{
				//最终释放，不要修改
				i = 0;
				step = 0;
				out_hand3 = 0;
				out_clear();
				JS_HARD_ON();
				key_enable = 1;
			}
			break;
		}
	}
	
	
	
	//黄脚宏
	if (out_foot1 == 1)
	{
		switch (step)
		{
		case 0:
			
			step = 1;
			break;
			
		case 1:
			
			i ++;
			
			if (i == 20)
			{
				//最终释放，不要修改
				i = 0;
				step = 0;
				out_foot1 = 0;
				out_clear();
				JS_HARD_ON();
				key_enable = 1;
			}
			break;
		}
	}
	
	
	//蓝脚宏
	if (out_foot2 == 1)
	{
		switch (step)
		{
		case 0:
			
			step = 1;
			break;
			
		case 1:
			
			i ++;
			
			if (i == 20)
			{
				//最终释放，不要修改
				i = 0;
				step = 0;
				out_foot2 = 0;
				out_clear();
				JS_HARD_ON();
				key_enable = 1;
			}
			break;
		}
	}
	
	
	//红脚宏
	if (out_foot3 == 1)
	{
		switch (step)
		{
		case 0:
			
			step = 1;
			break;
			
		case 1:
			
			i ++;
			
			if (i == 20)
			{
				//最终释放，不要自己修改
				i = 0;
				step = 0;
				out_foot3 = 0;
				out_clear();
				JS_HARD_ON();
				key_enable = 1;
			}
			break;
		}
	}
}






