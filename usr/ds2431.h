#ifndef _DS2431_H__
#define _DS2431_H__
#include "stm32f10x.h"


#define DS2431_H()		(GPIOB->BSRR = GPIO_Pin_15)
#define DS2431_L()		(GPIOB->BRR = GPIO_Pin_15)

#define DS2431_OUT()	GPIOB->CRH &= 0x0fffffff; GPIOB->CRH |= 0x70000000
#define DS2431_IN()		GPIOB->CRH &= 0x0fffffff; GPIOB->CRH |= 0x40000000

#define DS2431_READ()	(GPIOB->IDR & GPIO_Pin_15)


#define CMD_READ_ROM            0x33
#define CMD_SKIP_ROM            0xcc
#define CMD_READ_SCRATCHPAD     0xaa
#define CMD_READ_MEM            0xf0
#define CMD_WRITE_SCRATCHPAD    0x0f
#define CMD_COPY_SCRATCHPAD     0x55




void ds2431_init(void);
uint8_t ow_rst(void);
uint8_t read_mem(void);
uint8_t read_rom(void);
void send_cmd(uint8_t cmd);
uint8_t read_dat(uint8_t *pdat);
uint8_t ds2431_read(void);



#endif


