#include "cmsis_os.h"
#include "lcd.h"
#include "my_time.h"

void lcd_show_task(void const * argument)
{
	LCD_init();
    struct Time_Object lcd_time;
	
	for (;;)
	{
        get_time_period(&lcd_time);
        LCD_printf(4, 60, ST7735Ctx.Width, 16, 16, "T: %d", (uint32_t)lcd_time.Now_Time);
        LCD_printf(4, 78, ST7735Ctx.Width, 16, 16, "DT: %d", lcd_time.Time_Delta_INT);
		osDelay(100);
	}
}


