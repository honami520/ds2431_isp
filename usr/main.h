#ifndef _MAIN_H__
#define _MAIN_H__

#include "stm32f10x.h"
#include "out.h"
#include "key.h"





uint8_t time_1ms_flag;
uint8_t time_10ms_flag;
uint8_t time_100ms_flag;
uint8_t time_1000ms_flag;

uint8_t time_2ms_flag;




//key
uint16_t lb_num, rb_num;
uint8_t lb_flag, rb_flag;
uint8_t key_enable = 1;
uint16_t hand1_num, hand2_num, hand3_num;
uint8_t hand1_flag, hand2_flag, hand3_flag;
uint16_t foot1_num, foot2_num, foot3_num;
uint8_t foot1_flag, foot2_flag, foot3_flag;



//out
uint8_t out_flag1 = 0, out_flag2 = 0;
uint8_t out_state1 = 0, out_state2 = 0;
uint8_t out_hand1 = 0, out_hand2 = 0, out_hand3 = 0;
uint8_t out_foot1 = 0, out_foot2 = 0, out_foot3 = 0;


#endif


