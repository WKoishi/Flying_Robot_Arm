#include "servo_ctrl.h"
#include "usb_uart.h"

void servo_single_data_pack_send(uint8_t id, ServoCommand command, uint8_t* param_list, uint16_t param_len);
extern struct ServoBusManager bus_1_manager;

ServoObject::ServoObject(uint8_t id_)
{
    this->bus_manager = &bus_1_manager;
    this->_id = id_;
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
        num_retransmit = bus_manager->default_num_retransmit;
    
    bus_manager->inquiry_command = command_PING;
    bus_manager->inquiry_id = _id;
    manager_reset_respond_flag(bus_manager);
    
    command_ping();
    for (i = 1; i <= num_retransmit; i++)
    {
        ret_val = servo_wait_respond(bus_manager, bus_manager->wait_time_ms);
        if (true == ret_val)
        {
            return ret_val;
        }
        else
            command_ping();
    }
    ret_val = servo_wait_respond(bus_manager, bus_manager->wait_time_ms);
    
    return ret_val;
}

bool ServoObject::read_data_with_respond(ServoRegAddress address, uint8_t read_length, 
                    int8_t num_retransmit)
{
    uint8_t i = 0;
    uint8_t ret_val = false;
    
    if (DEFAULT_NUM_RETRANSMIT == num_retransmit)
        num_retransmit = bus_manager->default_num_retransmit;
    
    bus_manager->inquiry_command = command_READ_DATA;
    bus_manager->inquiry_id = _id;
    bus_manager->inquiry_address = address;
    bus_manager->receive_target = this;    // Let the manager store data to this class in the receive ISR
    manager_reset_respond_flag(bus_manager);
    
    command_read_data(address, read_length);
    for (i = 1; i <= num_retransmit; i++)
    {
        ret_val = servo_wait_respond(bus_manager, bus_manager->wait_time_ms);
        if (true == ret_val)
        {
            return ret_val;
        }
        else
            command_read_data(address, read_length);
    }
    ret_val = servo_wait_respond(bus_manager, bus_manager->wait_time_ms);
    
    return ret_val;
}

bool ServoObject::export_read_state(void)
{
    bus_manager->special_command = special_READ_STATE;
    return read_data_with_respond(servo_POSITION_NOW, 15, 1);
}



