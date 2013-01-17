#include "key.h"
#include "ds2431.h"
#include "FILE_SYS.H"
#include "tm1640.h"
#include "flash_eep.h"


extern uint8_t led_flag;				//LED标志，1为SUCCESS, 2为FAILED
extern uint8_t led_delay_num;			//LED延时

extern uint8_t rom_save[200][8];		//上电后保存到的所有ID号，ID不重复
extern uint8_t rom_save_num;			//上电后保存的ID号个数

extern uint8_t rom_buf[8];
extern uint8_t mem_buf[133];
extern uint8_t usb_buf[500];

extern uint16_t ds2431_num;
extern uint16_t stm8_num;

extern uint8_t err_state;
extern uint8_t usb_state;


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
	uint8_t rt = 0;
	uint16_t i;

	if (key1_flag == 1)
	{
		//读取DS2431数据
		key1_flag = 0;
		
		//关闭LED
		led_flag = 0;
		
		if ((err_state == 0) && (usb_state == 2))
		{
			//USB正常的时候，执行读取DS2431操作
			rt = ds2431_read();
			
			if (rt == 1)
			{
				//读取到数据，则对比ID号是否已经存在
				rt = rom_check();
				
				if (rt == 1)
				{
					//新的ID号，则写入U盘
					//打开文件，如果文件不存在则创建，同时改变DS2431已读个数，如果打开失败则报错，改变usb状态，退出
					rt = file_write();
					
					if (rt == 1)
					{
						//写入成功, DS2431读取数字增加，并写入EEPROM
						ds2431_num ++;
						eep_write16(REG_DS2431, ds2431_num);
						
						//成功标志
						led_flag = 1;
					}
					else if (rt == 0)
					{
						//写入失败,报故障，并且删除添加的ID号
						rom_save_num --;
						for (i = 0; i < 8; i ++)
						{
							rom_save[rom_save_num][i] = 0;
						}
						
						//故障标志
						led_flag = 2;
					}
				}
				else if(rt == 0)
				{
					//已经存在数据，则忽略，直接报成功
					//成功标志
					led_flag = 2;
				}
			}
			else if (rt == 0)
			{
				//读取数据失败，报故障
				led_flag = 2;
			}
			
			//清空rom_buf, mem_buf, usb_buf
			for (i = 0; i < 8; i ++)
			{
				rom_buf[i] = 0;
			}
			
			for (i = 0; i < 133; i ++)
			{
				mem_buf[i] = 0;
			}
			
			for (i = 0; i < 500; i ++)
			{
				usb_buf[i] = 0;
			}
		}
	}
	
	if (key2_flag == 1)
	{
		//写入STM8
		key2_flag = 0;
	}
	
	if (key3_flag == 1)
	{
		//复位STM8写入位置
		key3_flag = 0;
	}
}


//返回1表示OK，返回0表示已存在
uint8_t rom_check(void)
{
	uint8_t i, j;
	
	for (i = 0; i < rom_save_num; i ++)
	{
		for (j = 0; j < 8; j ++)
		{
			if (rom_buf[j] != rom_save[i][j])
			{
				break;
			}
		}
		
		if (j == 8)
		{
			//有相同ID，则跳出
			break;
		}
	}
	
	//有一致的数据时候，i < rom_save_num
	if (i == rom_save_num)
	{
		//没有匹配的ID号，则把新ID存入
		if (rom_save_num == 200)
		{
			rom_save_num = 200 - 1;
		}
		
		for (j = 0; j < 8; j ++)
		{
			rom_save[rom_save_num][j] = rom_buf[j];
		}
		
		//ID号个数增加
		rom_save_num ++;
		
		return 1;
	}
	else
	{	
		return 0;
	}
}


//1：写入成功；0：写入失败
uint8_t file_write(void)
{
	uint8_t i;
	uint8_t res = 0;
	uint16_t tmb_in = 0, tmb_real = 0;
	uint32_t file_size = 0;
	
	for (i = 0; i < 10; i ++)
	{
		res = CH376FileOpen("/DS2431.TXT");
		if (res == USB_INT_SUCCESS)
		{
			//文件打开成功，则跳出
			break;
		}
		else if (res == ERR_MISS_FILE)
		{
			//文件不存在，则创建
			res = CH376FileCreate("/DS2431.TXT");
			
			if (res == USB_INT_SUCCESS)
			{
				//文件创建成功，跳出
				CH376FileOpen("/DS2431.TXT");
				break;
			}
		}
		else
		{
			//延时再次尝试打开
			mdelay(100);
		}
	}
	
	if (i == 10)
	{
		//打开失败，直接返回0
		return 0;
	}
	else if (i < 10)
	{
		//打开文件成功，移动指针到最后
		file_size = CH376GetFileSize();
		
		if (file_size == 1)
		{
			file_size = 0;
		}
		
		CH376ByteLocate(file_size);
		
		//将rom_buf和mem_buf写入usb_buf
		tmb_in = usb_buf_in();
		
		//以字节写入数据
		CH376ByteWrite(usb_buf, tmb_in, &tmb_real);
		CH376FileClose(TRUE);
		
		return 1;
	}
	else
	{
		return 0;
	}
}


//把ROM_BUF, MEM_BUF按照规则填入USB_BUF，转变成AISCII形式
//返回值为写入字节数
uint16_t usb_buf_in(void)
{
	uint16_t i, num = 0;
	uint8_t tmp[2] = {0, 0};
	
	//写入ROM_BUF	
	usb_buf[num ++] = 'I';
	usb_buf[num ++] = 'D';
	usb_buf[num ++] = ':';
	usb_buf[num ++] = 0x09;
	
	for (i = 0; i < 8; i ++)
	{
		tmp[0] = rom_buf[i] >> 4;
		tmp[1] = (rom_buf[i] & 0x0f);
		
		if (tmp[0] < 10)
		{
			usb_buf[num ++] = 0x30 + tmp[0];
		}
		else
		{
			usb_buf[num ++] = 0x41 + tmp[0] - 10;
		}
		
		if (tmp[1] < 10)
		{
			usb_buf[num ++] = 0x30 + tmp[1];
		}
		else
		{
			usb_buf[num ++] = 0x41 + tmp[1] - 10;
		}		
		
		usb_buf[num ++] = 0x20;
	}
	
	usb_buf[num ++] = 0x0d;
	usb_buf[num ++] = 0x0a;
	
	//写入MEM0
	usb_buf[num ++] = 'M';
	usb_buf[num ++] = 'E';
	usb_buf[num ++] = 'M';
	usb_buf[num ++] = '0';
	usb_buf[num ++] = ':';
	usb_buf[num ++] = 0x09;
	
	for (i = 0; i < 32; i ++)
	{
		tmp[0] = mem_buf[i] >> 4;
		tmp[1] = (mem_buf[i] & 0x0f);
		
		if (tmp[0] < 10)
		{
			usb_buf[num ++] = 0x30 + tmp[0];
		}
		else
		{
			usb_buf[num ++] = 0x41 + tmp[0] - 10;
		}
		
		if (tmp[1] < 10)
		{
			usb_buf[num ++] = 0x30 + tmp[1];
		}
		else
		{
			usb_buf[num ++] = 0x41 + tmp[1] - 10;
		}	
		
		usb_buf[num ++] = 0x20;
	}
	
	usb_buf[num ++] = 0x0d;
	usb_buf[num ++] = 0x0a;	
	
	//写入MEM1
	usb_buf[num ++] = 'M';
	usb_buf[num ++] = 'E';
	usb_buf[num ++] = 'M';
	usb_buf[num ++] = '1';
	usb_buf[num ++] = ':';
	usb_buf[num ++] = 0x09;
	
	for (i = 0; i < 32; i ++)
	{
		tmp[0] = mem_buf[i + 32] >> 4;
		tmp[1] = (mem_buf[i + 32] & 0x0f);
		
		if (tmp[0] < 10)
		{
			usb_buf[num ++] = 0x30 + tmp[0];
		}
		else
		{
			usb_buf[num ++] = 0x41 + tmp[0] - 10;
		}
		
		if (tmp[1] < 10)
		{
			usb_buf[num ++] = 0x30 + tmp[1];
		}
		else
		{
			usb_buf[num ++] = 0x41 + tmp[1] - 10;
		}	
		
		usb_buf[num ++] = 0x20;
	}
	
	usb_buf[num ++] = 0x0d;
	usb_buf[num ++] = 0x0a;	
	
	return num;
}








