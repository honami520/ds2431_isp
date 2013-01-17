#include "flash_eep.h"
#include "eeprom.h"


extern uint16_t VirtAddVarTab[NumbOfVar];
extern uint16_t ds2431_num;
extern uint16_t stm8_num;


void Flash_Config(void)
{
	FLASH_Unlock();
	EE_Init();
}


void eep_write16(uint8_t reg, uint16_t val)
{
	EE_WriteVariable(0xaa00 + reg, val);
}

void eep_write32(uint8_t reg, uint32_t val)
{
	EE_WriteVariable(0xaa00 + reg, (uint16_t)(val >> 16));
	EE_WriteVariable(0xaa00 + reg + 1, (uint16_t)(val & 0x0000ffff));
}


uint16_t eep_read16(uint8_t reg)
{
	uint16_t rt = 0;
	
	EE_ReadVariable(0xaa00 + reg, &rt);
	
	return rt;
}


uint32_t eep_read32(uint8_t reg)
{
	uint32_t rt = 0;
	uint16_t tmp = 0;
	
	EE_ReadVariable(0xaa00 + reg, &tmp);
	rt = tmp;
	rt <<= 16;
	EE_ReadVariable(0xaa00 + reg + 1, &tmp);
	rt += tmp;
	
	return rt;
}



void eep_init(void)
{
	uint32_t pw = 0;
	
	Flash_Config();
	
	pw = eep_read32(REG_PASS);
	
	if (pw == PASSWORD)
	{
		//已经有数据，则读取
		ds2431_num = eep_read16(REG_DS2431);
		stm8_num = eep_read16(REG_STM8);
	}
	else
	{
		//无数据，写入初始化数据
		ds2431_num = 0;
		stm8_num = 0;
		
		eep_write16(REG_DS2431, ds2431_num);
		eep_write16(REG_STM8, stm8_num);
		eep_write32(REG_PASS, PASSWORD);
	}
}





