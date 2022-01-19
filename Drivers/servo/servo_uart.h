#ifndef _SERVO_UART_H_
#define _SERVO_UART_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SERVO_BUFFER_SIZE 512

void servo_receive_init(void);
void servo_send_data_hardware(uint8_t* data, uint16_t length);
    
#ifdef __cplusplus
}
#endif

#endif // _SERVO_UART_H_

