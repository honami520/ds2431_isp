#include "key.h"



extern uint16_t key1_num, key2_num, key3_num;
extern uint8_t key1_flag, key2_flag, key3_flag;

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}



void key_scan(void)
{
	if (KEY1_READ() == 0)
	{
		key1_num ++;
		
		if (key1_num == 5)
		{
			key1_flag = 1;
		}
	}
	else
	{
		key1_num = 0;
	}
	
	if (KEY2_READ() == 0)
	{
		key2_num ++;
		
		if (key2_num == 5)
		{
			key2_flag = 1;
		}
	}
	else
	{
		key2_num = 0;
	}
	
	if (KEY3_READ() == 0)
	{
		key3_num ++;
		
		if (key3_num == 5)
		{
			key3_flag = 1;
		}
	}
	else
	{
		key3_num = 0;
	}
	

}


void key_done(void)
{

	if (key1_flag == 1)
	{
		//
		key1_flag = 0;
	}
	
	if (key2_flag == 1)
	{
		//
		key2_flag = 0;
	}
	
	if (key3_flag == 1)
	{
		//
		key3_flag = 0;
	}
}






