#include "tm1640.h"



extern uint8_t seg_dat[28];
extern uint8_t seg_buf[16];

extern uint16_t ds2431_num;
extern uint16_t stm8_num;

extern uint8_t err_state;
extern uint8_t usb_state;


extern uint8_t led_flag;				//LED标志，1为SUCCESS, 2为FAILED
extern uint16_t led_delay_num;			//LED延时


void udelay(uint16_t i)
{
	uint16_t j;
	
	for (; i > 0; i --)
	{
		j = 10;
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
		j = 10000;
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
	
	/* PB6 PB7为输出*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	seg_init();
}



void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* PB6 PB7为输出*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	LED_SUC_OFF();
	LED_FAIL_OFF();
}




void tm1640_start(void)
{
    SDA_OUT();
    
    SDA_H();
    SCL_H();
    udelay(1);
    SDA_L();
    udelay(1);
    SCL_L();    //拉低
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



void seg_init(void)
{
	seg_handle();
}


void seg_handle(void)
{
	if ((err_state == 0) && (usb_state == 2))
	{
		//无故障的时候显示数字
		seg_buf[0] = seg_dat[ds2431_num / 1000];
		seg_buf[1] = seg_dat[(ds2431_num % 1000) / 100];
		seg_buf[2] = seg_dat[(ds2431_num % 100) / 10];
		seg_buf[3] = seg_dat[ds2431_num % 10];
		
		if (seg_buf[0] == seg_dat[SEG_0])
		{
			seg_buf[0] = seg_dat[SEG_BLANK];
			
			if (seg_buf[1] == seg_dat[SEG_0])
			{
				seg_buf[1] = seg_dat[SEG_BLANK];
				
				if (seg_buf[2] == seg_dat[SEG_0])
				{
					seg_buf[2] = seg_dat[SEG_BLANK];
				}
			}
		}
		
		seg_buf[4] = seg_dat[stm8_num / 1000];
		seg_buf[5] = seg_dat[(stm8_num % 1000) / 100];
		seg_buf[6] = seg_dat[(stm8_num % 100) / 10];
		seg_buf[7] = seg_dat[stm8_num % 10];
		
		if (seg_buf[4] == seg_dat[SEG_0])
		{
			seg_buf[4] = seg_dat[SEG_BLANK];
			
			if (seg_buf[5] == seg_dat[SEG_0])
			{
				seg_buf[5] = seg_dat[SEG_BLANK];
				
				if (seg_buf[6] == seg_dat[SEG_0])
				{
					seg_buf[6] = seg_dat[SEG_BLANK];
				}
			}
		}		
	}
	else if (err_state != 0)
	{
		seg_buf[0] = seg_dat[SEG_E];
		seg_buf[1] = seg_dat[SEG_R];
		seg_buf[2] = seg_dat[SEG_R];
		seg_buf[3] = seg_dat[SEG_BLANK];
		seg_buf[4] = seg_dat[SEG_BLANK];
		seg_buf[5] = seg_dat[SEG_BLANK];
		seg_buf[6] = seg_dat[SEG_BLANK];
		seg_buf[7] = seg_dat[SEG_BLANK];
	}
	else if (usb_state < 2)
	{
		seg_buf[0] = seg_dat[SEG_U];
		seg_buf[1] = seg_dat[SEG_5];
		seg_buf[2] = seg_dat[seg_B];
		seg_buf[3] = seg_dat[SEG_BLANK];
		seg_buf[4] = seg_dat[SEG_N];
		seg_buf[5] = seg_dat[SEG_O];
		seg_buf[6] = seg_dat[SEG_BLANK];
		seg_buf[7] = seg_dat[SEG_BLANK];
	}
	
	write_cmd(0x40);
	write_dat(0xc0, seg_buf);
	write_cmd(0x88);
}


//1ms进入一次
void led_handle(void)
{
	if (led_flag == 1)
	{
		LED_SUC_ON();
		LED_FAIL_OFF();
		led_delay_num ++;
		
		if (led_delay_num == 1000)
		{
			led_delay_num = 0;
			led_flag = 0;
			LED_SUC_OFF();
			LED_FAIL_OFF();
		}
	}
	else if (led_flag == 2)
	{
		LED_SUC_OFF();
		LED_FAIL_ON();
		led_delay_num ++;
		
		if (led_delay_num == 1000)
		{
			led_delay_num = 0;
			led_flag = 0;
			LED_SUC_OFF();
			LED_FAIL_OFF();
		}		
	}
	else if (led_flag == 0)
	{
		led_delay_num = 0;
		LED_SUC_OFF();
		LED_FAIL_OFF();
	}
}




