#ifndef _SERVO_LINK_H_
#define _SERVO_LINK_H_

#include "stdint.h"
#include "stdbool.h"
#include "my_time.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define servo_ID                    0X05
//#define servo_BAUDRATE              0X06
//#define servo_RESPONSE_DELAY        0X07
//#define servo_RESPONSE_STATE        0X08
//#define servo_ANGLE_LIMIT_MIN       0X09
//#define servo_ANGLE_LIMIT_MAX       0X0B
//#define servo_TEMPERATURE_MAX       0X0D
//#define servo_TORQUE_MAX            0X10
//#define servo_TRIGGER_UNLOAD        0X13
//#define servo_TRIGGER_LED_ALARM     0X14
//#define servo_CURRENT_PROTECT       0X1C
//#define servo_ANGLE_RESOLUTION      0X1E
//#define servo_POSITION_CORRECT      0X1F
//#define servo_MODE                  0X21
//#define servo_TORQUE_PROTECT        0X22
//#define servo_TIME_TORQUE_PROTECT   0X23
//#define servo_TORQUE_OVERLOAD       0X24
//#define servo_TIME_OVERCURRENT      0X26
//#define servo_TORQUE_SWITCH         0X28
//#define servo_ACCELERATION          0X29
//#define servo_POSITION_TARGET       0X2A
//#define servo_TIME_RUNTIME          0X2C
//#define servo_SPEED_TARGET          0X2E
//#define servo_TORQUE_LIMIT          0X30
//#define servo_POSITION_NOW          0X38
//#define servo_SPEED_NOW             0X3A
//#define servo_LOAD_NOW              0X3C
//#define servo_VOLTAGE_NOW           0X3E
//#define servo_TEMPERATURE_NOW       0X3F
//#define servo_STATE                 0X41
//#define servo_CURRENT_NOW           0X45

typedef enum {
    servo_ID                    = 0X05U,
    servo_BAUDRATE              = 0X06U,
    servo_RESPONSE_DELAY        = 0X07U,
    servo_RESPONSE_STATE        = 0X08U,
    servo_ANGLE_LIMIT_MIN       = 0X09U,
    servo_ANGLE_LIMIT_MAX       = 0X0BU,
    servo_TEMPERATURE_MAX       = 0X0DU,
    servo_TORQUE_MAX            = 0X10U,
    servo_TRIGGER_UNLOAD        = 0X13U,
    servo_TRIGGER_LED_ALARM     = 0X14U,
    servo_CURRENT_PROTECT       = 0X1CU,
    servo_ANGLE_RESOLUTION      = 0X1EU,
    servo_POSITION_CORRECT      = 0X1FU,
    servo_MODE                  = 0X21U,
    servo_TORQUE_PROTECT        = 0X22U,
    servo_TIME_TORQUE_PROTECT   = 0X23U,
    servo_TORQUE_OVERLOAD       = 0X24U,
    servo_TIME_OVERCURRENT      = 0X26U,
    servo_TORQUE_SWITCH         = 0X28U,
    servo_ACCELERATION          = 0X29U,
    servo_POSITION_TARGET       = 0X2AU,
    servo_TIME_RUNTIME          = 0X2CU,
    servo_SPEED_TARGET          = 0X2EU,
    servo_TORQUE_LIMIT          = 0X30U,
    servo_POSITION_NOW          = 0X38U,
    servo_SPEED_NOW             = 0X3AU,
    servo_LOAD_NOW              = 0X3CU,
    servo_VOLTAGE_NOW           = 0X3EU,
    servo_TEMPERATURE_NOW       = 0X3FU,
    servo_FLAG_ASYNC_WRITE      = 0X40U,
    servo_STATE                 = 0X41U,
    servo_FLAG_MOVE             = 0X42U,
    servo_CURRENT_NOW           = 0X45U,
    SERVO_MEMORY_END,
} ServoRegAddress;

typedef enum {
    command_PING            = 0X01U,
    command_READ_DATA       = 0X02U,
    command_WRITE_DATA      = 0X03U,
    command_REGWRITE_DATA   = 0X04U,
    command_ACTION          = 0X05U,
    command_SYNCREAD_DATA   = 0X82U,
    command_SYNCWRITE_DATA  = 0X83U,
} ServoCommand;

struct ServoBusReceiver {
    const uint16_t wait_time_ms;
    const uint8_t num_retransmit;
    
    uint8_t inquiry_id;
    ServoRegAddress inquiry_address;
    ServoCommand inquiry_command;
    volatile uint8_t respond_flag;
};

void servo_single_receive_data(const uint8_t* data_buf, const uint16_t receive_len);
bool servo_wait_respond(struct ServoBusReceiver* receiver, const uint16_t wait_ms);
void receiver_reset_respond_flag(struct ServoBusReceiver* receiver);
    
#ifdef __cplusplus
}
#endif

#endif // _SERVO_LINK_H_









