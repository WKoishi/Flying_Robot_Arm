#include "servo_ctrl.h"
#include "usb_uart.h"
#include "string.h"

bool servo_single_data_pack_send(struct ServoBusManager* manager, uint8_t id, 
    const uint8_t* data, uint16_t data_len, bool wait_flag, int8_t num_retransmit);
extern struct ServoBusManager bus_1_manager;

ServoObject::ServoObject(uint8_t id_)
{
    this->bus_manager = &bus_1_manager;
    this->_id = id_;
};


bool ServoObject::ping_with_respond(bool wait_flag, int8_t num_retransmit)
{
    uint8_t i = 0;
    uint8_t data[2] = {0};
    bool ret_val = false;
    
    bus_manager->inquiry_command = ServoCommand::PING;
    bus_manager->inquiry_id = _id;
    
    data[0] = static_cast<uint8_t>(ServoCommand::PING);
    ret_val = servo_single_data_pack_send(bus_manager, _id, data, 1, wait_flag, num_retransmit);
    
    return ret_val;
}

bool ServoObject::read_data_with_respond(ServoRegAddress address, uint8_t read_length, 
                    bool wait_flag, int8_t num_retransmit)
{
    uint8_t i = 0;
    uint8_t data[4] = {0};
    uint8_t ret_val = false;
    
    bus_manager->inquiry_command = ServoCommand::READ_DATA;
    bus_manager->inquiry_id = _id;
    bus_manager->inquiry_address = address;
    bus_manager->receive_target = this;    // Let the manager store data to this class in the receive ISR
    
    data[0] = static_cast<uint8_t>(ServoCommand::READ_DATA);
    data[1] = static_cast<uint8_t>(address);
    data[2] = read_length;
    ret_val = servo_single_data_pack_send(bus_manager, _id, data, 3, wait_flag, num_retransmit);
    
    return ret_val;
}

bool ServoObject::write_data(ServoRegAddress address, const uint8_t* data, uint8_t length, 
    bool wait_flag, int8_t num_retransmit)
{
    uint8_t ret_val = false;
    
    bus_manager->inquiry_command = ServoCommand::WRITE_DATA;
    bus_manager->inquiry_id = _id;
    
    send_data_buffer[0] = static_cast<uint8_t>(ServoCommand::WRITE_DATA);
    send_data_buffer[1] = static_cast<uint8_t>(address);
    memcpy(send_data_buffer + 2, data, length*sizeof(uint8_t));
    
    ret_val = servo_single_data_pack_send(bus_manager, _id, send_data_buffer, length+2, wait_flag, num_retransmit);
    
    return ret_val;
}

bool ServoObject::export_read_state(void)
{
    bus_manager->special_command = ServoSpecialCommand::READ_STATE;
    return read_data_with_respond(ServoRegAddress::POSITION_NOW, 15, true, 1);
}

bool ServoObject::export_torque_switch(bool switch_flag)
{
    uint8_t send_data[2] = {0};
    
    send_data[0] = (uint8_t)switch_flag;
    return write_data(ServoRegAddress::TORQUE_SWITCH, send_data, 1, true, DEFAULT_NUM_RETRANSMIT);
}

bool ServoObject::export_set_middle_position(void)
{
    uint8_t send_data[2] = {0};
    
    send_data[0] = 128U;
    return write_data(ServoRegAddress::TORQUE_SWITCH, send_data, 1, true, DEFAULT_NUM_RETRANSMIT);
}

bool ServoObject::set_position(uint16_t position_)
{
    uint8_t send_data[4] = {0};
    
    if (position_ > POSITION_RESOLUTION)
        position_ = POSITION_RESOLUTION;
    
    send_data[0] = position_ & 0XFF;
    send_data[1] = (position_ >> 8) & 0XFF;
    
    return write_data(ServoRegAddress::POSITION_TARGET, send_data, 2, true, 1);
}

bool ServoObject::set_accelerate(uint8_t accelerate_)
{
    uint8_t send_data[2] = {0};
    
    if (accelerate_ > 254)
        accelerate_ = 254;
    
    send_data[0] = accelerate_;
    
    return write_data(ServoRegAddress::ACCELERATION, send_data, 1, true, DEFAULT_NUM_RETRANSMIT);
}





