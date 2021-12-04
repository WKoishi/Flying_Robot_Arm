#ifndef _SERVO_LINK_H_
#define _SERVO_LINK_H_

#include "stdint.h"

#define SERVO_ID                    0X05
#define SERVO_BAUDRATE              0X06
#define SERVO_RESPONSE_DELAY        0X07
#define SERVO_RESPONSE_STATE        0X08
#define SERVO_ANGLE_LIMIT_MIN       0X09
#define SERVO_ANGLE_LIMIT_MAX       0X0B
#define SERVO_TEMPERATURE_MAX       0X0D
#define SERVO_TORQUE_MAX            0X10
#define SERVO_UNLOAD_TRIGGER        0X13
#define SERVO_LED_ALARM_TRIGGER     0X14
#define SERVO_CURRENT_PROTECT       0X1C
#define SERVO_ANGLE_RESOLUTION      0X1E
#define SERVO_POSITION_CORRECT      0X1F
#define SERVO_MODE                  0X21
#define SERVO_TORQUE_PROTECT        0X22
#define SERVO_TIME_TORQUE_PROTECT   0X23
#define SERVO_TORQUE_OVERLOAD       0X24
#define SERVO_TIME_OVERCURRENT      0X26
#define SERVO_TORQUE_SWITCH         0X28
#define SERVO_ACCELERATION          0X29
#define SERVO_POSITION_TARGET       0X2A
#define SERVO_TIME_RUNTIME          0X2C
#define SERVO_SPEED_TARGET          0X2E
#define SERVO_TORQUE_LIMIT          0X30
#define SERVO_POSITION_NOW          0X38
#define SERVO_SPEED_NOW             0X3A
#define SERVO_LOAD_NOW              0X3C
#define SERVO_VOLTAGE_NOW           0X3E
#define SERVO_TEMPERATURE_NOW       0X3F
#define SERVO_STATE                 0X41
#define SERVO_CURRENT_NOW           0X45


#endif // _SERVO_LINK_H_









