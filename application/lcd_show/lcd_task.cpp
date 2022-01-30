#include "cmsis_os.h"
#include "lcd.h"
#include "my_time.h"
#include "usb_uart.h"
#include "fonts.h"

struct StringHandler tick_message;

extern "C" void lcd_show_task(void const * argument)
{
    LCD_init();
    struct Time_Object lcd_time = {0, 0, 0, 0};
    tick_message.x0 = 4;
    tick_message.y0 = 60;
    tick_message.area_width = 100;
    tick_message.area_height = 30;
    tick_message.font = &Font_7x10;
    tick_message.color = COLOR_TIANYIBLUE;
    //tick_message.bgcolor = COLOR_BLACK;
    
    int count = 0;
    
    //get_time_period(&lcd_time);

    for (;;)
    {
        get_time_period(&lcd_time);
//        tick_message.x0 = 4;
//        tick_message.y0 = 60;
//        LCD_printf(&tick_message, "T: %d", (uint32_t)lcd_time.Now_Time);
        tick_message.x0 = 4;
        tick_message.y0 = 72;
        
        if (count < 10)
        {
            LCD_printf(&tick_message, "cuiowebnco");
        }
        else if (count >= 10 && count < 20)
        {
            LCD_printf(&tick_message, "ewvw0");
        }
        else
            count = 0;
       
        LCD_WriteChar(1, 1, 'M', &Font_11x18, COLOR_TIANYIBLUE, COLOR_BLACK);
        
        ++count;
        
        osDelay(50);
    }
}


