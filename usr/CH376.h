/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			CH376.h
** Last modified Date: 	2011-6-29
** Last Version: 		1.0
** Descriptions: 		None 
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		AVRman
** Created date: 		2011-6-29
** Version: 1.0
** Descriptions: 		First version
**
**------------------------------------------------------------------------------------------------------
** Modified by: 	
** Modified date: 	
** Version:
** Descriptions:  	
**
********************************************************************************************************/

#ifndef __CH376_H 
#define __CH376_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "CH376INC.h"


#define		ERR_USB_UNKNOWN		0xFA		/* 未知错误,不应该发生的情况,需检查硬件或者程序错误 */

/* Private define ------------------------------------------------------------*/
/* 用于定义CH376的片选引脚 */
#define RCC_APB2Periph_GPIO_CH376_CS      RCC_APB2Periph_GPIOA
#define CH376_CS_PORT                     GPIOA
#define CH376_CS_PIN                      GPIO_Pin_4

#define CH376_CS_LOW()                    GPIO_ResetBits(CH376_CS_PORT, CH376_CS_PIN)
#define CH376_CS_HIGH()                   GPIO_SetBits(CH376_CS_PORT, CH376_CS_PIN)

/* 用于定义CH376的上拉引脚 */
#define RCC_APB2Periph_GPIO_CH376_INT     RCC_APB2Periph_GPIOA
#define CH376_INT_PORT                    GPIOA
#define CH376_INT_PIN                     GPIO_Pin_3 

#define CH376_INT_IN                      GPIO_ReadInputDataBit(CH376_INT_PORT, CH376_INT_PIN)

/* Private function prototypes -----------------------------------------------*/
void delay_us(uint16_t i);
void delay_ms(uint16_t i);
void CH376_SPI_Init(void);
uint8_t CH376_ReadWriteByte(uint8_t data);
void CH376_WR_CMD_PORT( uint8_t cmd );
void CH376_WR_DAT_PORT( uint8_t dat );
uint8_t CH376_RD_DAT_PORT( void );
uint8_t CH376_HOST_INIT(void);
uint8_t wait_interrupt(void);
uint8_t READ_CH376_VERSION(void);
uint8_t CH376_DISK_CONNECT(void);
uint8_t CH376_DISK_MOUNT(void);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

