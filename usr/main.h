#ifndef _MAIN_H__
#define _MAIN_H__

#include "stm32f10x.h"
#include "key.h"
#include "tm1640.h"
#include "CH376.h"
#include"FILE_SYS.H"





uint8_t time_1ms_flag;
uint8_t time_10ms_flag;
uint8_t time_100ms_flag;
uint8_t time_1000ms_flag;


//数码管
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
	
uint8_t seg_buf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t seg_back[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t seg_blink[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};




//key
uint16_t key1_num = 0, key2_num = 0, key3_num = 0;
uint8_t key1_flag = 0, key2_flag = 0, key3_flag = 0;



//ch376
#define USB_BUF_LEN		1024
static uint8_t buf_read[USB_BUF_LEN];
P_FAT_DIR_INFO	pDir;
uint8_t buf[64];
uint8_t version,res,i;
uint32_t file_size;


#endif


