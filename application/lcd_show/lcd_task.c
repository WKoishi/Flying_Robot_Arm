#include "cmsis_os.h"
#include "lcd.h"
#include "my_time.h"

void lcd_show_task(void const * argument)
{
	LCD_Test();
    struct Time_Object lcd_time;
    uint8_t text[50];
	
	for (;;)
	{
        get_time_period(&lcd_time);
        sprintf((char *)&text, "T: %d", (uint32_t)lcd_time.Now_Time);
        LCD_ShowString(4, 60, ST7735Ctx.Width, 16, 16, text);
        sprintf((char *)&text, "DT: %d", lcd_time.Time_Delta_INT);
        LCD_ShowString(4, 78, ST7735Ctx.Width, 16, 16, text);
		osDelay(100);
	}
}


