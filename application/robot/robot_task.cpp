#include "cmsis_os.h"
#include "stdint.h"
#include "servo_ctrl.h"
#include "usb_uart.h"
#include "robot.h"
#include "arm_math.h"

ServoObject servo_1(0X01);
ServoObject servo_2(0X02);

RobotArm robot;

struct Time_Object servo_time;

extern "C" void servo_ctrl_task(void const * argument)
{
    uint8_t ret_val = 0;
    get_time_period(&servo_time);
    int16_t posi_target = 0;
    int8_t reserve_flag = 1;
    
    int i = 0;
    float sinin = 0;
    
    servo_2.export_read_state();
    posi_target = servo_2.get_position();
    
    if (!servo_2.set_accelerate(20))
        for (;;) { osDelay(1); }
    
    for (;;)
    {
        osDelay(20);
        
        get_time_period(&servo_time);
        ret_val = servo_2.set_position(posi_target);
        ret_val = servo_2.export_read_state();
        get_time_period(&servo_time);
        if (ret_val)
            usb_printf("Y1 %d,%d,%f,%f,%d,%f,%f\r\n", servo_2.position, servo_2.velocity, 
                servo_2.load_ratio, servo_2.voltage, servo_2.temperature, servo_2.current, servo_time.Time_Delta);
        else
            usb_printf("N1 %d,%d,%f,%f,%d,%f,%f\r\n", servo_2.position, servo_2.velocity, 
                servo_2.load_ratio, servo_2.voltage, servo_2.temperature, servo_2.current, servo_time.Time_Delta);
        
        posi_target += 10 * reserve_flag;
        
        if (posi_target > 4000)
        {
            reserve_flag = -1;
            posi_target = 4000;
        }
        else if (posi_target < 0)
        {
            reserve_flag = 1;
            posi_target = 0;
        }

        
//        get_time_period(&servo_time);
//        ret_val = servo_2.ping_with_respond(true, 0);
//        get_time_period(&servo_time);
//        if (ret_val)
//            usb_printf("Y2 %d,%d,%f,%f,%d,%f,%f\r\n", servo_2.position, servo_2.velocity, 
//                servo_2.load_ratio, servo_2.voltage, servo_2.temperature, servo_2.current, servo_time.Time_Delta);
//        else
//            usb_printf("N2 %d,%d,%f,%f,%d,%f,%f\r\n", servo_2.position, servo_2.velocity, 
//                servo_2.load_ratio, servo_2.voltage, servo_2.temperature, servo_2.current, servo_time.Time_Delta);
//        
//        usb_printf("%d %d", sizeof(*servo_1.bus_manager), sizeof(servo_1));

        //usb_printf("%f", servo_time.Time_Delta);
            
    }
}


