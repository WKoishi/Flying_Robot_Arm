#ifndef _SERVO_CTRL_H_
#define _SERVO_CTRL_H_

#include "stdint.h"
#include "stdbool.h"
#include "servo_link.h"

class ServoObject {
public:
    
    float position_target;
    float velocity_target;
    float accelerate;

    float load_ratio;
    float voltage;
    float current;
    uint16_t position;
    int16_t velocity;
    uint8_t temperature;
    uint8_t async_flag;
    uint8_t servo_state;
    uint8_t move_flag;
    
    struct ServoBusManager* bus_manager;
    
    ServoObject(const uint8_t id_);
    void export_init(const uint8_t id_);
    void write_data(ServoRegAddress address, const uint8_t* data, uint8_t length, bool wait_flag, int8_t num_retransmit);
    bool ping_with_respond(bool wait_flag, int8_t num_retransmit);
    bool read_data_with_respond(ServoRegAddress address, uint8_t read_length, bool wait_flag, int8_t num_retransmit);
    
    bool export_read_state(void);
    
    void export_get_angle(void);
    
    //write state data from uart receiver
    void record_position(uint16_t position_)
    {
        this->position = position_;
    }
    void record_velocity(int16_t velocity_)
    {
        if (velocity_ >= 0) this->velocity = velocity_;
        else this->velocity = 0X8000 - (uint16_t)velocity_;
    }
    void record_load_ratio(uint16_t load_ratio_)
    {
        this->load_ratio = (float)load_ratio_ * 0.001f;
    }
    void record_voltage(uint8_t voltage_)
    {
        this->voltage = (float)voltage_ * 0.1f;
    }
    void record_current(uint16_t current_)
    {
        this->current = (float)current_ * 0.0065f;
    }
    void record_temperature(uint8_t temperature_)
    {
        this->temperature = temperature_;
    }
    void record_async_flag(uint8_t async_flag_)
    {
        this->async_flag = async_flag_;
    }
    void record_servo_state(uint8_t servo_state_)
    {
        this->servo_state = servo_state_;
    }
    void record_move_flag(uint8_t move_flag_)
    {
        this->move_flag = move_flag_;
    }
    
    //read state data
    uint16_t get_position(void)
    {
        return position;
    }
    int16_t get_velocity(void)
    {
        return velocity;
    }
    float get_load_ratio(void)
    {
        return load_ratio;
    }
    float get_voltage(void)
    {
        return voltage;
    }
    float get_current(void)
    {
        return current;
    }
    uint8_t get_temperature(void)
    {
        return temperature;
    }
    uint8_t get_async_flag(void)
    {
        return async_flag;
    }
    uint8_t get_servo_state(void)
    {
        return servo_state;
    }
    uint8_t get_move_flag(void)
    {
        return move_flag;
    }
    
    
private:
    float _angle_limit_min;
    float _angle_limit_max; 
    uint8_t _id;

};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif  // _SERVO_CTRL_H_



