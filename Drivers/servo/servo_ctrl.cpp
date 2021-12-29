#include "servo_ctrl.h"
#include "usb_uart.h"

extern "C" void servo_single_data_pack_send(uint8_t id, ServoCommand command, uint8_t* param_list, uint16_t param_len);
extern struct ServoBusReceiver bus_1_receiver;

ServoObject::ServoObject(uint8_t id, float angle_limit_min, float angle_limit_max):
    _id(id), _angle_limit_min(angle_limit_min), _angle_limit_max(angle_limit_max)
{
    this->receiver = &bus_1_receiver;
};


void ServoObject::command_ping(void)
{
    uint8_t param_list[2] = {0};
    servo_single_data_pack_send(this->_id, command_PING, param_list, 0);
}

void ServoObject::command_read_data(ServoRegAddress address, uint8_t read_length)
{
    uint8_t param_list[2];
    
    param_list[0] = address;
    param_list[1] = read_length;
    servo_single_data_pack_send(this->_id, command_READ_DATA, param_list, 2);
}

bool ServoObject::ping_with_respond(void)
{
    uint8_t i = 0;
    uint8_t ret_val = false, wait_flag = false;
    
    receiver->inquiry_command = command_PING;
    receiver->inquiry_id = _id;
    receiver_reset_respond_flag(receiver);
    
    command_ping();
    for (i = 1; i <= receiver->num_retransmit; i++)
    {
        wait_flag = servo_wait_respond(receiver, receiver->wait_time_ms);
        if (true == wait_flag)
        {
            ret_val = true;
            break;
        }
        else
            command_ping();
    }
    
    return ret_val;
}

bool ServoObject::read_data_with_respond(ServoRegAddress address, uint8_t read_length)
{
    uint8_t i = 0;
    uint8_t ret_val = false;
    
    receiver->inquiry_command = command_READ_DATA;
    receiver->inquiry_id = _id;
    receiver->inquiry_address = address;
    receiver_reset_respond_flag(receiver);
    
    command_read_data(address, read_length);
    for (i = 1; i <= receiver->num_retransmit; i++)
    {
        if (true == servo_wait_respond(receiver, receiver->wait_time_ms))
        {
            ret_val = true;
            break;
        }
        else
            command_read_data(address, read_length);
    }
    
    return ret_val;
}

