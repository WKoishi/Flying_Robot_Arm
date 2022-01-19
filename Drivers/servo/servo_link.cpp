#include "servo_link.h"
#include "servo_uart.h"
#include "string.h"
#include "cmsis_os.h"

uint8_t servo_send_buffer[SERVO_BUFFER_SIZE];

struct ServoBusManager bus_1_manager = {
    .wait_time_ms = 100,
    .default_num_retransmit = 2,
    .receive_target = NULL,
};


static inline uint8_t manager_get_id(struct ServoBusManager* receiver)
{
    return receiver->inquiry_id;
}

static inline ServoRegAddress manager_get_address(struct ServoBusManager* receiver)
{
    return receiver->inquiry_address;
}

static inline ServoCommand manager_get_command(struct ServoBusManager* receiver)
{
    return receiver->inquiry_command;
}

static inline bool manager_get_respond_flag(struct ServoBusManager* receiver)
{
    return receiver->respond_flag;
}

static void manager_set_respond_flag(struct ServoBusManager* receiver)
{
    receiver->respond_flag = true;
}

void manager_reset_respond_flag(struct ServoBusManager* receiver)
{
    receiver->respond_flag = false;
}

void servo_single_data_pack_send(uint8_t id, ServoCommand command, uint8_t* param_list, uint16_t param_len)
{
    uint16_t sum = 0, i = 0;
    
    if (param_len > SERVO_BUFFER_SIZE - 10)
        return;
    
    servo_send_buffer[0] = 0XFF;
    servo_send_buffer[1] = 0XFF;
    servo_send_buffer[2] = id;
    servo_send_buffer[3] = param_len + 2;
    servo_send_buffer[4] = command;
    
    //memcpy(servo_send_buffer + 5, param, param_len);  //Not necessary
    
    for (i = 0; i < param_len; i++)
    {
        servo_send_buffer[i + 5] = param_list[i];
        sum += param_list[i];
    }
    sum += id;
    sum += param_len + 2;
    sum += command;
    servo_send_buffer[param_len + 5] = ~(uint8_t)(sum & 0XFF);
    
    servo_send_data_hardware(servo_send_buffer, param_len + 6);
}

extern "C" void servo_single_receive_data_ISR(const uint8_t* data_buf, const uint16_t receive_len)
{
    uint16_t i = 0;
    uint8_t sum = 0;
    ServoCommand command;
    
    if (receive_len < 4 || data_buf[0] != 0XFF || data_buf[1] != 0XFF)
        return;
    
    for (i = 2; i < receive_len - 1; i++)
    {
        sum += data_buf[i];
    }
    sum = ~sum;
    if (sum == data_buf[receive_len - 1])
    {
        command = manager_get_command(&bus_1_manager);
        
        if (command_PING == command)
        {
            if (manager_get_id(&bus_1_manager) == data_buf[2] && 6 == receive_len)
                manager_set_respond_flag(&bus_1_manager);
        }
        else if (command_READ_DATA == command)
        {
            if (special_READ_STATE == bus_1_manager.special_command)
            {
                if (manager_get_id(&bus_1_manager) == data_buf[2] && 21 == receive_len )
                {
                    if (bus_1_manager.receive_target != NULL)
                    {
                        bus_1_manager.receive_target->record_position   (data_buf[5] | data_buf[6]<<8);
                        bus_1_manager.receive_target->record_velocity   (data_buf[7] | data_buf[8]<<8);
                        bus_1_manager.receive_target->record_load_ratio (data_buf[9] | data_buf[10]<<8);
                        bus_1_manager.receive_target->record_voltage    (data_buf[11]);
                        bus_1_manager.receive_target->record_temperature(data_buf[12]);
                        bus_1_manager.receive_target->record_async_flag (data_buf[13]);
                        bus_1_manager.receive_target->record_servo_state(data_buf[14]);
                        bus_1_manager.receive_target->record_move_flag  (data_buf[15]);
                        bus_1_manager.receive_target->record_current    (data_buf[18] | data_buf[19]<<8);
                        
                        manager_set_respond_flag(&bus_1_manager);
                        bus_1_manager.receive_target = NULL;  //reset the point
                    }
                }
                bus_1_manager.special_command = special_NONE;
            }
        }
    }
}

bool servo_wait_respond(struct ServoBusManager* receiver, const uint16_t wait_ms)
{
    bool ret_val = false;
    uint32_t tick = 0;
    
    for (tick = 0; tick < wait_ms; tick++)
    {
        osDelay(1);
        if (true == manager_get_respond_flag(receiver))
        {
            ret_val = true;
            break;
        }
    }
    return ret_val;
}

//void servo_command_write_data(uint8_t id, ServoRegAddress address)
//{
//    
//}








