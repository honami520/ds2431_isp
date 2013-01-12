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

void delay_us(uint16_t i)
{
	uint8_t j;
	for(;i > 0;i --)
	{
		for(j = 72;j > 0;j --);
	}
}

void delay_ms(uint16_t i)
{
	while(i --)
	{
		delay_us(100);
	}
}

/*******************************************************************************
* Function Name  : CH376_SPI_Init
* Description    : ��ʼ���ܽ�
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
* Description    : ͨ��Ӳ��SPI����һ���ֽڵ�CH376
* Input          : - data: ���͵�����
* Output         : None
* Return         : CH376 ���ص�����
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
* Description    : дCH376�����Ӻ���
* Input          : - cmd: 8λ������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CH376_WR_CMD_PORT( uint8_t cmd )
{
	CH376_CS_HIGH();               /* ��ֹ֮ǰδͨ��xEndCH376Cmd��ֹSPIƬѡ */
	delay_us(1);
	CH376_CS_LOW();                /* Ƭѡ��Ч */
	delay_us(1);
	CH376_ReadWriteByte( cmd );    /* ���������� */
}

/*******************************************************************************
* Function Name  : CH376_WR_DAT_PORT
* Description    : дCH376�����Ӻ���
* Input          : - dat: 8λ����
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CH376_WR_DAT_PORT( uint8_t dat )
{
	CH376_ReadWriteByte( dat );   /* ���������� */
}

/*******************************************************************************
* Function Name  : CH376_RD_DAT_PORT
* Description    : ��CH376�����Ӻ���
* Input          : None
* Output         : None
* Return         : 8λ����
* Attention		 : None
*******************************************************************************/
uint8_t CH376_RD_DAT_PORT( void )
{
    return(CH376_ReadWriteByte( 0 ));
}

/*******************************************************************************
* Function Name  : wait_interrupt
* Description    : �ȴ��ж� ���һ�ȡ�ж�״̬	
* Input          : None
* Output         : None
* Return         : �ж�״̬
* Attention		 : CH376��������ж�(INT#�͵�ƽ) 
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
	uint8_t res,i;

	for(i = 0;i < 5;i ++)
	{
		CH376_WR_CMD_PORT(CMD0H_DISK_CONNECT);
		res = wait_interrupt();
		if(res == USB_INT_SUCCESS)
		{
			break;
		}
		delay_ms(50);
	}
	CH376_CS_HIGH();
	return res;
}

uint8_t CH376_DISK_MOUNT(void)
{
	uint8_t res,i;

	for(i = 0;i < 5;i ++)
	{
		CH376_WR_CMD_PORT(CMD0H_DISK_MOUNT);
		res = wait_interrupt();
		if(res == USB_INT_SUCCESS)
		{
			break;
		}
		delay_ms(50);
		
	}
	CH376_CS_HIGH();
	return res;
}

/*******************************************************************************
* Function Name  : Set_USB_Mode
* Description    : ����CH376�Ĺ���ģʽ 
* Input          : - mode: ģʽ����
* Output         : None
* Return         : ����״̬ TRUE���ɹ���FALSEʧ��
* Attention		 : 0x06Ϊ����ģʽ
*******************************************************************************/
uint8_t CH376_HOST_INIT(void) 
{  
	uint8_t res;
	
	CH376_SPI_Init( );                             //�ӿ�Ӳ����ʼ��
	CH376_WR_CMD_PORT( CMD11_CHECK_EXIST );        //���Ե�Ƭ����CH376֮���ͨѶ�ӿ�
	CH376_WR_DAT_PORT( 0x65 );
	res = CH376_RD_DAT_PORT( );
	CH376_CS_HIGH();
	if ( res != 0x9A ) 
	{
		return( ERR_USB_UNKNOWN );          		//ͨѶ�ӿڲ�����
	}

	CH376_WR_CMD_PORT( CMD11_SET_USB_MODE );        //�豸USB����ģʽ
	CH376_WR_DAT_PORT( 0x06 );
	delay_ms( 1 );
	res = CH376_RD_DAT_PORT( );
	CH376_CS_HIGH( );

	if ( res == CMD_RET_SUCCESS ) 
	{
	    return( USB_INT_SUCCESS );					//USBģʽ���óɹ�					
	}
	else 
	{
	    return( ERR_USB_UNKNOWN );                   //����ģʽ����
	}
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

