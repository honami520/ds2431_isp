#ifndef FLASH_EEP_H__
#define FLASH_EEP_H__

#include "stm32f10x.h"

#define PASSWORD	0x1f2e3c4d
#define REG_PASS	0
#define REG_DS2431	2
#define REG_STM8	3


void eep_write16(uint8_t reg, uint16_t val);
void eep_write32(uint8_t reg, uint32_t val);
uint16_t eep_read16(uint8_t reg);
uint32_t eep_read32(uint8_t reg);
void eep_init(void);

#endif



