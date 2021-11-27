#include "my_time.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;
static uint32_t get_hal_base_timer_cnt(void)
{
    return __HAL_TIM_GetCounter(&htim2);
}

void get_time_period(struct Time_Object *time_obj)
{
    uint32_t tim_cnt = get_hal_base_timer_cnt();
    
    time_obj->Last_Time = time_obj->Now_Time;
    time_obj->Now_Time = (HAL_GetTick() + (float)tim_cnt/1000.0f) / 1000.0f;  //��λs
    time_obj->Time_Delta = (time_obj->Now_Time - time_obj->Last_Time);
    time_obj->Time_Delta_INT = (uint16_t)(time_obj->Time_Delta);
}


