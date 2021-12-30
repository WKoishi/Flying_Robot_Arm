#ifndef _SERVO_CTRL_H_
#define _SERVO_CTRL_H_

#include "stdint.h"
#include "stdbool.h"
#include "servo_link.h"

class ServoObject {
public:
    const uint8_t _id;
    const float _angle_limit_min;
    const float _angle_limit_max;
    
    float position_target;
    float velocity_target;
    float accelerate;
    
    struct ServoBusReceiver* receiver;

    ServoObject(uint8_t id, float angle_limit_min, float angle_limit_max);
    void command_ping(void);
    void command_read_data(ServoRegAddress address, uint8_t read_length);
    bool ping_with_respond(void);
    bool read_data_with_respond(ServoRegAddress address, uint8_t read_length);
    
    //write private data from uart receiver
    void record_position(uint16_t position_)
    {
        this->position = position_;
    }
    void record_velocity(uint16_t velocity_)
    {
        this->velocity = velocity_;
    }
    void record_load_ratio(float load_ratio_)
    {
        this->load_ratio = load_ratio_;
    }
    void record_voltage(float voltage_)
    {
        this->voltage = voltage_;
    }
    void record_current(float current_)
    {
        this->current = current_;
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
    
    //read private data
    uint16_t get_position(void)
    {
        return position;
    }
    uint16_t get_velocity(void)
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
    uint16_t position;
    uint16_t velocity;
    float load_ratio;
    float voltage;
    float current;
    uint8_t temperature;
    uint8_t async_flag;
    uint8_t servo_state;
    uint8_t move_flag;           
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif  // _SERVO_CTRL_H_



