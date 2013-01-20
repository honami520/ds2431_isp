#include "stm8.h"
#include "flash_eep.h"

extern uint8_t led_flag;
extern uint16_t stm8_num;

extern uint8_t send_buf[100];
extern uint8_t rec_buf[50];
extern uint8_t send_buf_num;
extern uint8_t rec_buf_num;
extern uint8_t send_buf_flag;
extern uint16_t send_buf_delay;

extern uint8_t rom_buf[8];
extern uint8_t mem_buf[133];

extern uint8_t clear_flag;
extern uint16_t clear_num;

void stm8_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//开启GPIOA,UART1,AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 TX DMA1 Channel (triggered by USART1 Tx event) Config */
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)send_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	
	//使能TX DMA中断，优先级为1
 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


//STM8发送程序
//将rom_buf, mem_buf的数据发送给DMA
void stm8_write(void)
{
	uint8_t i;
	
	send_buf_num = 0;
	send_buf[send_buf_num ++] = 0xaa;
	send_buf[send_buf_num ++] = 0xbb;
	
	for (i = 0; i < 8; i ++)
	{
		send_buf[send_buf_num ++] = rom_buf[i];
	}
	
	for (i = 0; i < 64; i ++)
	{
		send_buf[send_buf_num ++] = mem_buf[i];
	}
	
	//和校验写入
	sum_add();
	
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, send_buf_num);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	
	send_buf_num = 0;
}


//计算校验和，将其写入对应的位置，更新数据长度
void sum_add(void)
{
	uint16_t tmp = 0;
	uint8_t i;
	
	for (i = 0; i < send_buf_num; i ++)
	{
		tmp += send_buf[i];
	}
	
	send_buf[send_buf_num ++] = (uint8_t)(tmp >> 8);
	send_buf[send_buf_num ++] = (uint8_t)(tmp & 0xff);
}


void uart_rec_decode(uint8_t val)
{
	uint8_t i;
	uint16_t tmp[2] = {0, 0};
	
	rec_buf[rec_buf_num ++] = val;
	
	//开启清零函数
	clear_flag = 1;
	clear_num = 0;
	
	if (rec_buf_num == 4)
	{
		//收到4个字节，进行判断
		if ((rec_buf[0] == 0xaa) && (rec_buf[1] == 0xbb))
		{
			//头一样，进行校验
			tmp[0] = 0xaa + 0xbb;
			tmp[1] = rec_buf[2];
			tmp[1] <<= 8;
			tmp[1] += rec_buf[3];
			
			if (tmp[0] == tmp[1])
			{
				//校验成功
				if (send_buf_flag == 1)
				{
					//写入成功
					stm8_num ++;
					eep_write16(REG_STM8, stm8_num);
					
					send_buf_flag = 0;
					send_buf_delay = 0;
					led_flag = 1;
				}
			}
		}
		
		//清除
		clear_flag = 0;
		clear_num = 0;
		rec_buf_num = 0;
		
		for (i = 0; i < 10; i ++)
		{
			rec_buf[i] = 0;
		}
	}
}

//1ms进行一次，1秒钟没有收到信息则清零
void uart_clear(void)
{
    uint8_t i;

    if (clear_flag == 1)
    {
        //清零延时计数
        clear_num++;
		
        if (clear_num >= 1000)
        {
            //延时满，执行清除操作
            clear_num = 0;
            clear_flag = 0;
			
            rec_buf_num = 0;
			
            //清除串口接收
            for (i = 0; i < 10; i++)
            {
                rec_buf[i] = 0;
            }
        }
    }
}



