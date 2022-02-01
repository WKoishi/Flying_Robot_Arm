#ifndef _SERVO_LINK_H_
#define _SERVO_LINK_H_

#include "stdint.h"
#include "stdbool.h"
#include "my_time.h"

enum class ServoRegAddress {
    ID                    = 0X05U,
    BAUDRATE              = 0X06U,
    RESPONSE_DELAY        = 0X07U,
    RESPONSE_STATE        = 0X08U,
    ANGLE_LIMIT_MIN       = 0X09U,
    ANGLE_LIMIT_MAX       = 0X0BU,
    TEMPERATURE_MAX       = 0X0DU,
    TORQUE_MAX            = 0X10U,
    TRIGGER_UNLOAD        = 0X13U,
    TRIGGER_LED_ALARM     = 0X14U,
    CURRENT_PROTECT       = 0X1CU,
    ANGLE_RESOLUTION      = 0X1EU,
    POSITION_CORRECT      = 0X1FU,
    MODE                  = 0X21U,
    TORQUE_PROTECT        = 0X22U,
    TIME_TORQUE_PROTECT   = 0X23U,
    TORQUE_OVERLOAD       = 0X24U,
    TIME_OVERCURRENT      = 0X26U,
    TORQUE_SWITCH         = 0X28U,
    ACCELERATION          = 0X29U,
    POSITION_TARGET       = 0X2AU,
    TIME_RUNTIME          = 0X2CU,
    SPEED_TARGET          = 0X2EU,
    TORQUE_LIMIT          = 0X30U,
    POSITION_NOW          = 0X38U,
    SPEED_NOW             = 0X3AU,
    LOAD_NOW              = 0X3CU,
    VOLTAGE_NOW           = 0X3EU,
    TEMPERATURE_NOW       = 0X3FU,
    FLAG_ASYNC_WRITE      = 0X40U,
    STATE                 = 0X41U,
    FLAG_MOVE             = 0X42U,
    CURRENT_NOW           = 0X45U,
    SERVO_MEMORY_END,
};

enum class ServoCommand {
    PING            = 0X01U,
    READ_DATA       = 0X02U,
    WRITE_DATA      = 0X03U,
    REGWRITE_DATA   = 0X04U,
    ACTION          = 0X05U,
    SYNCREAD_DATA   = 0X82U,
    SYNCWRITE_DATA  = 0X83U,
};

enum class ServoSpecialCommand {
    NONE = 0,
    READ_STATE,
};

#define DEFAULT_NUM_RETRANSMIT (-1)

#include "servo_ctrl.h"

struct ServoBusManager {
    ServoRegAddress inquiry_address;
    ServoCommand inquiry_command;
    ServoSpecialCommand special_command;
    
    const uint16_t wait_time_ms;
    const int8_t default_num_retransmit;
    
    uint8_t inquiry_id;
    volatile uint8_t respond_flag;
    
    class ServoObject* receive_target;  // The destination of the received data store
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
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

#endif // _SERVO_LINK_H_









