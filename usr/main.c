#include "main.h"



void systick_init(void)
{
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
}


int main(void)
{
	uint8_t i;
	
	systick_init();
	key_init();
	tm1640_init();
	led_init();
	mdelay(100);
	CH376_HOST_INIT();
	
	for (i = 0; i < 10; i ++)
	{
		if(CH376DiskConnect() == USB_INT_SUCCESS)
		{
			if(USB_INT_SUCCESS == CH376DiskMount())
			{
				break;
			}
			else
			{
				mdelay(200);
			}
		}	
		else
		{
			mdelay(500);
		}
	}
	
	while (1)
	{
		if (time_1ms_flag == 1)
		{
			time_1ms_flag = 0;
			key_scan();
			key_done();
		}
		
		if (time_10ms_flag == 1)
		{
			time_10ms_flag = 0;
		}
		
		if (time_100ms_flag == 1)
		{
			time_100ms_flag = 0;
			sg_write();
		}
		
		if (time_1000ms_flag == 1)
		{
			time_1000ms_flag = 0;
		}
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
