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
    
    void record_position(uint16_t position)
    {
        this->position = position;
    }
    uint16_t get_position(void)
    {
        return this->position;
    }
    void record_velocity(uint16_t velocity)
    {
        this->velocity = velocity;
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



