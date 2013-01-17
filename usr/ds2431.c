#include "ds2431.h"
#include "tm1640.h"



extern uint8_t rom_buf[8];
extern uint8_t mem_buf[133];


void ds2431_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* PB6 PB7Ϊ���*/
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
    
    //���500us�͵�ƽ
    DS2431_OUT();
    DS2431_L();
    //��ʱ500us
    udelay(550);
    //������
    DS2431_IN();
    
    time_out = 214;
    //����������ÿ��ռ��1.4usʱ��
    while(DS2431_READ() != 0)
    {
        time_out --;
        udelay(1);
        if(time_out == 0)
        {
            //ʧ�ܷ���0
            return 0;
        }
    }
    
    //15us - 60us
    if((time_out < 204) && (time_out > 171))
    {  
        time_out = 642;
        //while���ռ��1.4us
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
            //׼����64λROM
            time_out = 0;
            udelay(1);
            DS2431_IN();
            return 1;
        }
    }
    
    return rt;
}


//����1�ɹ�������0ʧ��
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
			//��ʧ���򷵻�0
			for (j = 0; j < 133; j ++)
			{
				mem_buf[j] = 0;
			}
			
			return 0;
		}
    }
	
	//�ɹ�����1
	return 1;
}

//����1�ɹ�������0ʧ��
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
			//��ȡʧ�ܷ���0
			for (j = 0; j < 8; j ++)
			{
				rom_buf[j] = 0;
			}
			
			return 0;
		}
    }    
	
	//��ȡ�ɹ�����1
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
            //д1
            DS2431_H();
        }
        else
        {
            //д0
            DS2431_L();
        }
        
        udelay(60);
        //�ͷ�IO 5us
        DS2431_H();
        udelay(5);
        
        cmd >>= 1;
    }
    
    DS2431_IN();
}



//����1��ʾ�ɹ�������0��ʾʧ��
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
            //����1
            tmp |= 0x80;
        }
        
        udelay(40);
        //�ȴ��ͷ�����
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
    
    //�ͷ�����
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
			//ROM��ȡ�ɹ������ȡMEM
			rt = read_mem();
			
			if (rt == 1)
			{
				//����ȡ�ɹ������˳�
				return 1;
			}
		}
	}
	
	//��ȡʧ��
	return 0;
}






