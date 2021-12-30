#include "servo_link.h"
#include "servo_uart.h"
#include "string.h"
#include "cmsis_os.h"

uint8_t servo_send_buffer[SERVO_BUFFER_SIZE];

struct ServoBusReceiver bus_1_receiver = {
    .wait_time_ms = 500,
    .num_retransmit = 2,
};

static uint8_t receiver_get_id(struct ServoBusReceiver* receiver)
{
    return receiver->inquiry_id;
}

static ServoRegAddress receiver_get_address(struct ServoBusReceiver* receiver)
{
    return receiver->inquiry_address;
}

static ServoCommand receiver_get_command(struct ServoBusReceiver* receiver)
{
    return receiver->inquiry_command;
}

static bool receiver_get_respond_flag(struct ServoBusReceiver* receiver)
{
    return receiver->respond_flag;
}

static void receiver_set_respond_flag(struct ServoBusReceiver* receiver)
{
    receiver->respond_flag = true;
}

void receiver_reset_respond_flag(struct ServoBusReceiver* receiver)
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
    
    servo_send_data_dma(servo_send_buffer, param_len + 6);
}

void servo_single_receive_data(const uint8_t* data_buf, const uint16_t receive_len)
{
    uint16_t i = 0;
    uint8_t sum = 0;
    
    if (receive_len < 4 || data_buf[0] != 0XFF || data_buf[1] != 0XFF)
        return;
    
    for (i = 2; i < receive_len - 1; i++)
    {
        sum += data_buf[i];
    }
    sum = ~sum;
    if (sum == data_buf[receive_len - 1])
    {
        if (receiver_get_command(&bus_1_receiver) == command_PING)
        {
            if (receiver_get_id(&bus_1_receiver) == data_buf[2] && receive_len == 6)
                receiver_set_respond_flag(&bus_1_receiver);
        }
    }
}

bool servo_wait_respond(struct ServoBusReceiver* receiver, uint16_t wait_ms)
{
    bool ret_val = false;
    uint32_t tick = 0;
    
    wait_ms /= 2U;
    for (tick = 0; tick < wait_ms; tick++)
    {
        if (true == receiver_get_respond_flag(receiver))
        {
            ret_val = true;
            break;
        }
        osDelay(1);
    }
    
    return ret_val;
}

//void servo_command_write_data(uint8_t id, ServoRegAddress address)
//{
//    
//}








