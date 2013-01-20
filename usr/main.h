#ifndef _MAIN_H__
#define _MAIN_H__

#include "stm32f10x.h"
#include "key.h"
#include "tm1640.h"
#include "CH376.h"
#include "FILE_SYS.H"
#include "eeprom.h"
#include "flash_eep.h"
#include "ds2431.h"
#include "stm8.h"




uint8_t time_1ms_flag;
uint8_t time_10ms_flag;
uint8_t time_100ms_flag;
uint8_t time_1000ms_flag;


//eeprom
uint16_t VirtAddVarTab[NumbOfVar] = {
	0xaa00, 0xaa01, 0xaa02, 0xaa03,
};

uint16_t ds2431_num = 0;
uint16_t stm8_num = 0;


//数码管
uint8_t seg_dat[28] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x6F, // 9
	0x77, // A
	0x7C, // b
	0x39, // C
	0x5E, // d
	0x79, // E
	0x71, // F
	0x3D, // G
	0x76, // H
	0x0E, // J
	0x38, // L
	0x37, // N
	0x5C, // o
	0x73, // P
	0x50, // r
	0x3E, // U
	0x6E, // Y
	0x40, // -
	0x00  // 不显示
};
	
uint8_t seg_buf[16] = {0, 0, 0, 0x3F, 0, 0, 0, 0x3F, 0, 0, 0, 0, 0, 0, 0, 0};


//ds2431
uint8_t rom_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t mem_buf[133] = {0};
uint8_t usb_buf[500] = {0};			//存放asic2形式的rom_buf，mem_buf，以及回车等信息

uint8_t rom_save[200][8] = {0};		//上电后保存到的所有ID号，ID不重复
uint8_t rom_save_num = 0;			//上电后保存的ID号个数


//led
uint8_t led_flag = 0;				//LED标志，1为SUCCESS, 2为FAILED
uint16_t led_delay_num = 0;			//LED延时


//key
uint16_t key1_num = 0, key2_num = 0, key3_num = 0;
uint8_t key1_flag = 0, key2_flag = 0, key3_flag = 0;



//ch376
#define USB_BUF_LEN		1024
uint8_t buf_read[USB_BUF_LEN];
P_FAT_DIR_INFO	pDir;
uint8_t buf[64];
uint8_t version,res,i;

uint8_t usb_state = 0;			//USB连接挂载状态	0:未连接		1:连接但未挂载		2:连接已挂载
uint8_t err_state = 0;			//故障类型



//stm8
uint8_t send_buf[100] = {0};
uint8_t send_buf_num = 0;
uint8_t rec_buf[50] = {0};
uint8_t rec_buf_num = 0;
uint8_t send_buf_flag = 0;
uint16_t send_buf_delay = 0;


//uart
uint8_t clear_flag = 0;
uint16_t clear_num = 0;

#endif


