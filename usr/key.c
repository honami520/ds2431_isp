#include "key.h"
#include "out.h"



extern uint16_t lb_num, rb_num;
extern uint8_t lb_flag, rb_flag;
extern uint8_t key_enable;
extern uint8_t out_flag1, out_flag2;
extern uint8_t out_state1, out_state2;

extern uint16_t hand1_num, hand2_num, hand3_num;
extern uint8_t hand1_flag, hand2_flag, hand3_flag;
extern uint16_t foot1_num, foot2_num, foot3_num;
extern uint8_t foot1_flag, foot2_flag, foot3_flag;
extern uint8_t out_hand1, out_hand2, out_hand3;
extern uint8_t out_foot1, out_foot2, out_foot3;


void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}



void key_scan(void)
{
	if (key_enable == 1)
	{
		if (LB_READ() == 0)
		{
			lb_num ++;
			
			if (lb_num == 1)
			{
				lb_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();
			}
		}
		else
		{
			lb_num = 0;
		}
		
		if (RB_READ() == 0)
		{
			rb_num ++;
			
			if (rb_num == 1)
			{
				rb_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();
			}
		}
		else
		{
			rb_num = 0;
		}
		
		//ª∆»≠≈–∂œ
		if (HAND1_READ() == 0)
		{
			hand1_num ++;
			
			//ª∆»≠≥÷–¯∂‡…Ÿms«–ªª
			if (hand1_num == 1)
			{
				hand1_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();				
			}
		}
		else
		{
			hand1_num = 0;
		}
		
		//¿∂»≠≈–∂œ
		if (HAND2_READ() == 0)
		{
			hand2_num ++;
			
			//¿∂»≠≥÷–¯∂‡…Ÿms«–ªª
			if (hand2_num == 1)
			{
				hand2_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();				
			}
		}
		else
		{
			hand2_num = 0;
		}
		
		//∫Ï»≠≈–∂œ
		if (HAND3_READ() == 0)
		{
			hand3_num ++;
			
			//∫Ï»≠≥÷–¯∂‡…Ÿms«–ªª
			if (hand3_num == 1)
			{
				hand3_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();				
			}
		}
		else
		{
			hand3_num = 0;
		}
		
		//ª∆Ω≈≈–∂œ
		if (FOOT1_READ() == 0)
		{
			foot1_num ++;
			
			//ª∆Ω≈≥÷–¯∂‡…Ÿms«–ªª
			if (foot1_num == 1)
			{
				foot1_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();
			}
		}
		else
		{
			foot1_num = 0;
		}
		
		//¿∂Ω≈≈–∂œ
		if (FOOT2_READ() == 0)
		{
			foot2_num ++;
			
			//¿∂Ω≈≥÷–¯∂‡…Ÿms«–ªª
			if (foot2_num == 1)
			{
				foot2_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();
			}
		}
		else
		{
			foot2_num = 0;
		}		
		
		//∫ÏΩ≈≈–∂œ
		if (FOOT3_READ() == 0)
		{
			foot3_num ++;
			
			//∫ÏΩ≈≥÷–¯∂‡…Ÿms«–ªª
			if (foot3_num == 1)
			{
				foot3_flag = 1;
				key_enable = 0;
				JS_MONI_ON();
				out_clear();
			}
		}
		else
		{
			foot3_num = 0;
		}
		
	}
}


void key_done(void)
{
	if (rb_flag == 1)
	{
		rb_flag = 0;
		out_state1 = 2;
		out_flag1 = 1;
		
		if (out_state1 == 3)
		{
			//2’–∑¢ÕÍ£¨—≠ª∑∑¢’–
			out_state1 = 1;
		}
	}
	
	if (lb_flag == 1)
	{
		lb_flag = 0;
		out_state2 = 1;
		out_flag2 = 1;
	}
	
	if (hand1_flag == 1)
	{
		hand1_flag = 0;
		out_hand1 = 1;
	}
	
	if (hand2_flag == 1)
	{
		hand2_flag = 0;
		out_hand2 = 1;
	}
	
	if (hand3_flag == 1)
	{
		hand3_flag = 0;
		out_hand3 = 1;
	}
	
	if (foot1_flag == 1)
	{
		foot1_flag = 0;
		out_foot1 = 1;
	}
	
	if (foot2_flag == 1)
	{
		foot2_flag = 0;
		out_foot2 = 1;
	}
	
	if (foot3_flag == 1)
	{
		foot3_flag = 0;
		out_foot3 = 1;
	}
}






