/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			CH376.c
** Descriptions:		None 
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-6-29
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "CH376.h"
#include "tm1640.h"
#include "flash_eep.h"
#include "FILE_SYS.H"

extern uint8_t usb_state;				//USB连接挂载状态	0:未连接		1:连接但未挂载		2:连接已挂载
extern uint8_t err_state;
extern uint16_t ds2431_num;

/*******************************************************************************
* Function Name  : CH376_SPI_Init
* Description    : 初始化管脚
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CH376_SPI_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable SPI1 and GPIOA clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_CH376_CS | RCC_APB2Periph_GPIO_CH376_INT, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* CH376 CS */
	GPIO_InitStructure.GPIO_Pin = CH376_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CH376_CS_PORT, &GPIO_InitStructure);

    /* CH376 INT */
    GPIO_InitStructure.GPIO_Pin = CH376_INT_PIN;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;   
    GPIO_Init(CH376_INT_PORT, &GPIO_InitStructure);
	
	CH376_CS_HIGH();
	/* SPI1 Config -------------------------------------------------------------*/ 								  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	/* Enable SPI1 */ 
	SPI_Cmd(SPI1, ENABLE); 
}


/*******************************************************************************
* Function Name  : CH376_ReadWriteByte
* Description    : 通过硬件SPI发送一个字节到CH376
* Input          : - data: 发送的数据
* Output         : None
* Return         : CH376 返回的数据
* Attention		 : None
*******************************************************************************/
uint8_t CH376_ReadWriteByte(uint8_t data)		
{
	uint8_t i;
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, data);
	
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	i = 10;
	while(i > 0)
	{
		i -- ;		
	} 
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : CH376_WR_CMD_PORT
* Description    : 写CH376命令子函数
* Input          : - cmd: 8位命令码
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CH376_WR_CMD_PORT( uint8_t cmd )
{
	CH376_CS_HIGH();               /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	udelay(1);
	CH376_CS_LOW();                /* 片选有效 */
	udelay(1);
	CH376_ReadWriteByte( cmd );    /* 发出命令码 */
}

/*******************************************************************************
* Function Name  : CH376_WR_DAT_PORT
* Description    : 写CH376数据子函数
* Input          : - dat: 8位数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CH376_WR_DAT_PORT( uint8_t dat )
{
	CH376_ReadWriteByte( dat );   /* 发出命令码 */
}

/*******************************************************************************
* Function Name  : CH376_RD_DAT_PORT
* Description    : 读CH376数据子函数
* Input          : None
* Output         : None
* Return         : 8位数据
* Attention		 : None
*******************************************************************************/
uint8_t CH376_RD_DAT_PORT( void )
{
    return(CH376_ReadWriteByte( 0 ));
}

/*******************************************************************************
* Function Name  : wait_interrupt
* Description    : 等待中断 并且获取中断状态	
* Input          : None
* Output         : None
* Return         : 中断状态
* Attention		 : CH376操作完成中断(INT#低电平) 
*******************************************************************************/
uint8_t wait_interrupt(void) 
{  
	while( CH376_INT_IN ); 
	CH376_WR_CMD_PORT( CMD_GET_STATUS ); 
	return( CH376_RD_DAT_PORT() );
}

uint8_t READ_CH376_VERSION(void)
{
	uint8_t res;

	CH376_WR_CMD_PORT(CMD01_GET_IC_VER);
	res = CH376_RD_DAT_PORT();
	CH376_CS_HIGH();
	return res;	
}

uint8_t CH376_DISK_CONNECT(void)
{
	uint8_t res;

	CH376_WR_CMD_PORT(CMD0H_DISK_CONNECT);
	res = wait_interrupt();
	CH376_CS_HIGH();
	
	return res;
}

uint8_t CH376_DISK_MOUNT(void)
{
	uint8_t res;

	CH376_WR_CMD_PORT(CMD0H_DISK_MOUNT);
	res = wait_interrupt();
	CH376_CS_HIGH();
	
	return res;
}

/*******************************************************************************
* Function Name  : Set_USB_Mode
* Description    : 设置CH376的工作模式 
* Input          : - mode: 模式代码
* Output         : None
* Return         : 操作状态 TRUE：成功，FALSE失败
* Attention		 : 0x06为主机模式
*******************************************************************************/
uint8_t CH376_HOST_INIT(void) 
{  
	uint8_t res;
	
	CH376_SPI_Init();                             //接口硬件初始化
	CH376_WR_CMD_PORT(CMD11_CHECK_EXIST);        //测试单片机与CH376之间的通讯接口
	CH376_WR_DAT_PORT(0x65);
	res = CH376_RD_DAT_PORT();
	CH376_CS_HIGH();
	if (res != 0x9A) 
	{
		err_state = 1;
		return(ERR_USB_UNKNOWN);          			//通讯接口不正常
	}

	CH376_WR_CMD_PORT(CMD11_SET_USB_MODE);        //设备USB工作模式
	CH376_WR_DAT_PORT(0x06);
	mdelay(1);
	res = CH376_RD_DAT_PORT();
	CH376_CS_HIGH();

	if (res == CMD_RET_SUCCESS) 
	{
	    return(USB_INT_SUCCESS);				//USB模式设置成功					
	}
	else 
	{
		err_state = 2;							//故障2，设置USB工作模式失败
	    return(ERR_USB_UNKNOWN);                //设置模式错误
	}
}



//USB插入和拔出检测，带自动挂载功能
//100ms进入一次
void usb_check_handle(void)
{
	uint8_t ret = 0;
	uint32_t tsize = 0;
	uint8_t i;
	
	if (err_state == 0)
	{
		//CH376无故障时候，检查U盘状况
		if (usb_state == 1)
		{
			//U盘已经连接，未挂载
			ret = CH376_DISK_MOUNT();
			
			if (ret == USB_INT_SUCCESS)
			{
				//U盘连接成功
				ret = 0;
				usb_state = 2;
				
				for (i = 0; i < 10; i ++)
				{
					ret = CH376FileOpen("/DS2431.TXT");
					if (ret == USB_INT_SUCCESS)
					{
						//文件打开成功，则跳出
						break;
					}
					else if (ret == ERR_MISS_FILE)
					{
						//文件不存在，则创建
						ret = CH376FileCreate("/DS2431.TXT");
						
						if (ret == USB_INT_SUCCESS)
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
				
				if (i < 10)
				{
					//文件已经打开
					tsize = CH376GetFileSize();
					
					//计算DS2431记录个数，写入EEPROM
					ds2431_num = tsize / 238;
					eep_write16(REG_DS2431, ds2431_num);
					
					CH376FileClose(FALSE);
				}
			}
		}
		else if (usb_state == 0)
		{
			//U盘未连接
			ret = CH376_DISK_CONNECT();
			
			if (ret == USB_INT_SUCCESS)
			{
				//U盘连接成功
				ret = 0;
				usb_state = 1;
			}
		}
	}
}


void usb_nc_handle(void)
{
	uint8_t res = 0;
	
	//USB断开检测
	if (usb_state == 2)
	{
		res = CH376_DISK_CONNECT();
		
		if (res == ERR_DISK_DISCON)
		{
			usb_state = 0;
		}
	}
}


/*********************************************************************************************************
    END FILE
*********************************************************************************************************/

