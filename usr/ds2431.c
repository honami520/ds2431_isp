#include "ds2431.h"
#include "tm1640.h"



extern uint8_t rom_buf[8];
extern uint8_t mem_buf[133];


void ds2431_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* PB6 PB7为输出*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	DS2431_H();
}


uint8_t ow_rst(void)
{
    uint8_t rt = 0;
	uint16_t time_out;
    
    //输出500us低电平
    DS2431_OUT();
    DS2431_L();
    //延时500us
    udelay(550);
    //输入检测
    DS2431_IN();
    
    time_out = 214;
    //下面这段语句每次占用1.4us时间
    while(DS2431_READ() != 0)
    {
        time_out --;
        udelay(1);
        if(time_out == 0)
        {
            //失败返回0
            return 0;
        }
    }
    
    //15us - 60us
    if((time_out < 204) && (time_out > 171))
    {  
        time_out = 642;
        //while语句占用1.4us
        while(DS2431_READ() == 0)
        {
            time_out --;
            udelay(1);
            if(time_out == 0)
            {
                return 0;
            }
        }
        
        //60us - 240us
        if((time_out > 470) && (time_out < 599))
        {
            //准备读64位ROM
            time_out = 0;
            udelay(1);
            DS2431_IN();
            return 1;
        }
    }
    
    return rt;
}


//返回1成功，返回0失败
uint8_t read_mem(void) 
{
    uint8_t i, j, rt = 0;
    
//    ow_rst();
//    send_cmd(CMD_SKIP_ROM); 
    send_cmd(CMD_READ_MEM);
    send_cmd(0x00);
    send_cmd(0x00);
    
    for(i = 0; i < 133; i ++)
    {
        rt = read_dat((mem_buf + i));
		
		if (rt == 0)
		{
			//读失败则返回0
			for (j = 0; j < 133; j ++)
			{
				mem_buf[j] = 0;
			}
			
			return 0;
		}
    }
	
	//成功返回1
	return 1;
}

//返回1成功，返回0失败
uint8_t read_rom(void)
{
    uint8_t i, j, rt = 0;
    
    ow_rst();
    send_cmd(CMD_READ_ROM);
    
    for(i = 0; i < 8; i ++)
    {
        rt = read_dat((rom_buf + i));
		
		if (rt == 0)
		{
			//读取失败返回0
			for (j = 0; j < 8; j ++)
			{
				rom_buf[j] = 0;
			}
			
			return 0;
		}
    }    
	
	//读取成功返回1
	return 1;
}


void send_cmd(uint8_t cmd)
{
    uint8_t i;

    DS2431_H();
    DS2431_OUT();
    
    for(i = 0; i < 8; i ++)
    {
        DS2431_L();
        udelay(5);
        
        if((cmd & 0x01) != 0)
        {
            //写1
            DS2431_H();
        }
        else
        {
            //写0
            DS2431_L();
        }
        
        udelay(60);
        //释放IO 5us
        DS2431_H();
        udelay(5);
        
        cmd >>= 1;
    }
    
    DS2431_IN();
}



//返回1表示成功，返回0表示失败
uint8_t read_dat(uint8_t *pdat)
{
    uint8_t i, tmp = 0;
	uint16_t time_out;

    for(i = 0; i < 8; i ++)
    {
        DS2431_L();
        DS2431_OUT();
        udelay(2);
        
        DS2431_L();
        DS2431_IN();
        udelay(10);        
        
        tmp >>= 1;
        if(DS2431_READ() != 0)
        {
            //读到1
            tmp |= 0x80;
        }
        
        udelay(40);
        //等待释放总线
		time_out = 10000;
        while(DS2431_READ() == 0)
		{
			time_out --;
			__NOP();
			
			if (time_out == 0)
			{
				return 0;
			}
		}
    }
    
    //释放总线
    DS2431_IN();
	*pdat = tmp;
    
    return 1;
}



uint8_t ds2431_read(void)
{
	uint8_t i, rt = 0;
	
	for (i = 0; i < 10; i ++)
	{
		ow_rst();
		udelay(9000);
		
		rt = read_rom();
		
		if (rt == 1)
		{
			//ROM读取成功，则读取MEM
			rt = read_mem();
			
			if (rt == 1)
			{
				//都读取成功，则退出
				return 1;
			}
		}
	}
	
	//读取失败
	return 0;
}






