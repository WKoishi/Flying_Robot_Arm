#include "cmsis_os.h"
#include "lcd.h"

void lcd_show_task(void const * argument)
{
	LCD_Test();
	
	for (;;)
	{
		osDelay(100);
	}
}


