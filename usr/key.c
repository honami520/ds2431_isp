#include "key.h"
#include "ds2431.h"
#include "FILE_SYS.H"
#include "tm1640.h"
#include "flash_eep.h"


extern uint8_t led_flag;				//LED��־��1ΪSUCCESS, 2ΪFAILED
extern uint8_t led_delay_num;			//LED��ʱ

extern uint8_t rom_save[200][8];		//�ϵ�󱣴浽������ID�ţ�ID���ظ�
extern uint8_t rom_save_num;			//�ϵ�󱣴��ID�Ÿ���

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
		//��ȡDS2431����
		key1_flag = 0;
		
		//�ر�LED
		led_flag = 0;
		
		if ((err_state == 0) && (usb_state == 2))
		{
			//USB������ʱ��ִ�ж�ȡDS2431����
			rt = ds2431_read();
			
			if (rt == 1)
			{
				//��ȡ�����ݣ���Ա�ID���Ƿ��Ѿ�����
				rt = rom_check();
				
				if (rt == 1)
				{
					//�µ�ID�ţ���д��U��
					//���ļ�������ļ��������򴴽���ͬʱ�ı�DS2431�Ѷ������������ʧ���򱨴��ı�usb״̬���˳�
					rt = file_write();
					
					if (rt == 1)
					{
						//д��ɹ�, DS2431��ȡ�������ӣ���д��EEPROM
						ds2431_num ++;
						eep_write16(REG_DS2431, ds2431_num);
						
						//�ɹ���־
						led_flag = 1;
					}
					else if (rt == 0)
					{
						//д��ʧ��,�����ϣ�����ɾ����ӵ�ID��
						rom_save_num --;
						for (i = 0; i < 8; i ++)
						{
							rom_save[rom_save_num][i] = 0;
						}
						
						//���ϱ�־
						led_flag = 2;
					}
				}
				else if(rt == 0)
				{
					//�Ѿ��������ݣ�����ԣ�ֱ�ӱ��ɹ�
					//�ɹ���־
					led_flag = 2;
				}
			}
			else if (rt == 0)
			{
				//��ȡ����ʧ�ܣ�������
				led_flag = 2;
			}
			
			//���rom_buf, mem_buf, usb_buf
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
		//д��STM8
		key2_flag = 0;
	}
	
	if (key3_flag == 1)
	{
		//��λSTM8д��λ��
		key3_flag = 0;
	}
}


//����1��ʾOK������0��ʾ�Ѵ���
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
			//����ͬID��������
			break;
		}
	}
	
	//��һ�µ�����ʱ��i < rom_save_num
	if (i == rom_save_num)
	{
		//û��ƥ���ID�ţ������ID����
		if (rom_save_num == 200)
		{
			rom_save_num = 200 - 1;
		}
		
		for (j = 0; j < 8; j ++)
		{
			rom_save[rom_save_num][j] = rom_buf[j];
		}
		
		//ID�Ÿ�������
		rom_save_num ++;
		
		return 1;
	}
	else
	{	
		return 0;
	}
}


//1��д��ɹ���0��д��ʧ��
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
			//�ļ��򿪳ɹ���������
			break;
		}
		else if (res == ERR_MISS_FILE)
		{
			//�ļ������ڣ��򴴽�
			res = CH376FileCreate("/DS2431.TXT");
			
			if (res == USB_INT_SUCCESS)
			{
				//�ļ������ɹ�������
				CH376FileOpen("/DS2431.TXT");
				break;
			}
		}
		else
		{
			//��ʱ�ٴγ��Դ�
			mdelay(100);
		}
	}
	
	if (i == 10)
	{
		//��ʧ�ܣ�ֱ�ӷ���0
		return 0;
	}
	else if (i < 10)
	{
		//���ļ��ɹ����ƶ�ָ�뵽���
		file_size = CH376GetFileSize();
		
		if (file_size == 1)
		{
			file_size = 0;
		}
		
		CH376ByteLocate(file_size);
		
		//��rom_buf��mem_bufд��usb_buf
		tmb_in = usb_buf_in();
		
		//���ֽ�д������
		CH376ByteWrite(usb_buf, tmb_in, &tmb_real);
		CH376FileClose(TRUE);
		
		return 1;
	}
	else
	{
		return 0;
	}
}


//��ROM_BUF, MEM_BUF���չ�������USB_BUF��ת���AISCII��ʽ
//����ֵΪд���ֽ���
uint16_t usb_buf_in(void)
{
	uint16_t i, num = 0;
	uint8_t tmp[2] = {0, 0};
	
	//д��ROM_BUF	
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
	
	//д��MEM0
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
	
	//д��MEM1
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








