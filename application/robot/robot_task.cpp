#include "cmsis_os.h"
#include "stdint.h"
#include "servo_ctrl.h"
#include "usb_uart.h"
#include "joint.hpp"

ServoObject servo_1(0X01);
ServoObject servo_2(0X02);

Joint<ServoObject> test(0X05);

struct Time_Object servo_time;

extern "C" void servo_ctrl_task(void const * argument)
{
    uint8_t ret_val = 0;
    get_time_period(&servo_time);
    
    for (;;)
    {
        osDelay(50);
        
        get_time_period(&servo_time);
        ret_val = servo_1.export_read_state();
        get_time_period(&servo_time);
        if (ret_val)
            usb_printf("Y1 %d,%d,%f,%f,%d,%f,%f\r\n", servo_1.position, servo_1.velocity, 
                servo_1.load_ratio, servo_1.voltage, servo_1.temperature, servo_1.current, servo_time.Time_Delta);
        else
            usb_printf("N1 %d,%d,%f,%f,%d,%f,%f\r\n", servo_1.position, servo_1.velocity, 
                servo_1.load_ratio, servo_1.voltage, servo_1.temperature, servo_1.current, servo_time.Time_Delta);

        
        get_time_period(&servo_time);
        ret_val = servo_2.export_read_state();
        get_time_period(&servo_time);
        if (ret_val)
            usb_printf("Y2 %d,%d,%f,%f,%d,%f,%f\r\n", servo_2.position, servo_2.velocity, 
                servo_2.load_ratio, servo_2.voltage, servo_2.temperature, servo_2.current, servo_time.Time_Delta);
        else
            usb_printf("N2 %d,%d,%f,%f,%d,%f,%f\r\n", servo_2.position, servo_2.velocity, 
                servo_2.load_ratio, servo_2.voltage, servo_2.temperature, servo_2.current, servo_time.Time_Delta);
        
        //usb_printf("%d %d", sizeof(*servo_1.bus_manager), sizeof(servo_1));
            
    }
}


