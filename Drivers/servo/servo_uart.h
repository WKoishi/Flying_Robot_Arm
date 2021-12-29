#ifndef _SERVO_UART_H_
#define _SERVO_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define SERVO_BUFFER_SIZE 512

void servo_receive_init(void);
void servo_send_data_dma(uint8_t* data, uint16_t length);
    
#ifdef __cplusplus
}
#endif

#endif // _SERVO_UART_H_

