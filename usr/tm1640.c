#include "tm1640.h"



extern uint8_t seg_dat[28];
extern uint8_t seg_buf[16];
extern uint8_t seg_back[16];
extern uint8_t seg_blink[16];



void udelay(uint16_t i)
{
	uint16_t j;
	
	for (; i > 0; i --)
	{
		j = 12;
		while(j > 0)
		{
			j --;
		}
	}
}

void mdelay(uint16_t i)
{
    uint16_t j;
    for(; i > 0; i --)
	{
		j = 12000;
		while(j > 0)
		{
			j --;
		}		
	}
}


void tm1640_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* PB6 PB7ÎªÊä³ö*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}




void tm1640_start(void)
{
    SDA_OUT();
    
    SDA_H();
    SCL_H();
    udelay(1);
    SDA_L();
    udelay(1);
    SCL_L();    //À­µÍ
    udelay(1);
}

void tm1640_stop(void)
{
    SDA_OUT();
    
    SDA_L();
    SCL_H();
    udelay(1);
    SDA_H();
    udelay(1);   
    SCL_L();
    udelay(1);
}

void tm1640_write_byte(uint8_t dat)
{
    uint8_t i, temp;
	
    SDA_OUT();
    for(i = 0; i < 8; i ++)
    {
        temp = dat & 0x01;
        if(temp)
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        SCL_H();
        udelay(1);
        SCL_L();
        udelay(1);
        dat >>= 1;
    }   
	
	udelay(1);
}


void write_cmd(uint8_t cmd)
{
    tm1640_start();
    tm1640_write_byte(cmd);
    tm1640_stop();	
}

void write_dat(uint8_t cmd, uint8_t *buf)
{
	uint8_t i;
	
    tm1640_start();
    tm1640_write_byte(cmd);
	
	for (i = 0; i < 16; i ++)
	{
		tm1640_write_byte(*(buf + i));
	}
    
    tm1640_stop();	
}


void sg_write(void)
{ 
	write_cmd(0x40);
	write_dat(0xc0, seg_buf);
	write_cmd(0x88);
}



void seg_blink_handle(void)
{
	static uint8_t step = 0;
	uint8_t i;
	
	for (i = 0; i < 16; i ++)
	{
		if (seg_blink[i] == 1)
		{
			if (step == 0)
			{
				seg_buf[i] = seg_back[i];
			}
			else
			{
				seg_buf[i] = 0x00;
			}
		}
	}
	
	step ++;
	
	if (step == 2)
	{
		step = 0;
	}
}





