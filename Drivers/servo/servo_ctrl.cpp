#include "servo_ctrl.h"
#include "usb_uart.h"

bool servo_single_data_pack_send(struct ServoBusManager* manager, uint8_t id, 
    const uint8_t* data, uint16_t data_len, bool wait_flag, int8_t num_retransmit);
extern struct ServoBusManager bus_1_manager;

ServoObject::ServoObject(uint8_t id_)
{
    this->bus_manager = &bus_1_manager;
    this->_id = id_;
};


void ServoObject::write_data(ServoRegAddress address, const uint8_t* data, uint8_t length, 
    bool wait_flag, int8_t num_retransmit)
{
    
}

bool ServoObject::ping_with_respond(bool wait_flag, int8_t num_retransmit)
{
    uint8_t i = 0;
    uint8_t data[2] = {0};
    bool ret_val = false;
    
    bus_manager->inquiry_command = command_PING;
    bus_manager->inquiry_id = _id;
    
    data[0] = command_PING;
    ret_val = servo_single_data_pack_send(bus_manager, _id, data, 1, wait_flag, num_retransmit);
    
    return ret_val;
}

bool ServoObject::read_data_with_respond(ServoRegAddress address, uint8_t read_length, 
                    bool wait_flag, int8_t num_retransmit)
{
    uint8_t i = 0;
    uint8_t data[4] = {0};
    uint8_t ret_val = false;
    
    bus_manager->inquiry_command = command_READ_DATA;
    bus_manager->inquiry_id = _id;
    bus_manager->inquiry_address = address;
    bus_manager->receive_target = this;    // Let the manager store data to this class in the receive ISR
    
    data[0] = command_READ_DATA;
    data[1] = address;
    data[2] = read_length;
    ret_val = servo_single_data_pack_send(bus_manager, _id, data, 3, wait_flag, num_retransmit);
    
    return ret_val;
}

bool ServoObject::export_read_state(void)
{
    bus_manager->special_command = special_READ_STATE;
    return read_data_with_respond(servo_POSITION_NOW, 15, true, 1);
}



