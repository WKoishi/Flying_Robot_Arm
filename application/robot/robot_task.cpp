#include "cmsis_os.h"
#include "stdint.h"
#include "servo_ctrl.h"
#include "usb_uart.h"

ServoObject servo_1(0X01, 0, 0);
ServoObject servo_2(0X02, 0, 0);

struct Time_Object servo_time;

extern "C" void servo_ctrl_task(void const * argument)
{
    uint8_t ret_val = 0;
    get_time_period(&servo_time);
    
    for (;;)
    {
        osDelay(10);
        
        get_time_period(&servo_time);
        ret_val = servo_1.ping_with_respond(DEFAULT_NUM_RETRANSMIT);
        get_time_period(&servo_time);
        if (ret_val)
            usb_printf("1 receive! %f\r\n", servo_time.Time_Delta);
        else
            usb_printf("1 no %f\r\n", servo_time.Time_Delta);
        
        get_time_period(&servo_time);
        ret_val = servo_2.ping_with_respond(DEFAULT_NUM_RETRANSMIT);
        get_time_period(&servo_time);
        if (ret_val)
            usb_printf("2 receive! %f\r\n", servo_time.Time_Delta);
        else
            usb_printf("2 no %f\r\n", servo_time.Time_Delta);
            
    }
}


