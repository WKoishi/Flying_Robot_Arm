#include "servo_ctrl.h"
#include "usb_uart.h"

void servo_single_data_pack_send(uint8_t id, ServoCommand command, uint8_t* param_list, uint16_t param_len);
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

bool ServoObject::ping_with_respond(int8_t num_retransmit)
{
    uint8_t i = 0;
    bool ret_val = false;
    
    if (DEFAULT_NUM_RETRANSMIT == num_retransmit)
        num_retransmit = receiver->default_num_retransmit;
    
    receiver->inquiry_command = command_PING;
    receiver->inquiry_id = _id;
    receiver_reset_respond_flag(receiver);
    
    command_ping();
    for (i = 1; i <= num_retransmit; i++)
    {
        ret_val = servo_wait_respond(receiver, receiver->wait_time_ms);
        if (true == ret_val)
        {
            return ret_val;
        }
        else
            command_ping();
    }
    ret_val = servo_wait_respond(receiver, receiver->wait_time_ms);
    
    return ret_val;
}

bool ServoObject::read_data_with_respond(ServoRegAddress address, uint8_t read_length, 
                    int8_t num_retransmit)
{
    uint8_t i = 0;
    uint8_t ret_val = false;
    
    if (DEFAULT_NUM_RETRANSMIT == num_retransmit)
        num_retransmit = receiver->default_num_retransmit;
    
    receiver->inquiry_command = command_READ_DATA;
    receiver->inquiry_id = _id;
    receiver->inquiry_address = address;
    receiver_reset_respond_flag(receiver);
    
    command_read_data(address, read_length);
    for (i = 1; i <= num_retransmit; i++)
    {
        ret_val = servo_wait_respond(receiver, receiver->wait_time_ms);
        if (true == ret_val)
        {
            return ret_val;
        }
        else
            command_read_data(address, read_length);
    }
    ret_val = servo_wait_respond(receiver, receiver->wait_time_ms);
    
    return ret_val;
}


