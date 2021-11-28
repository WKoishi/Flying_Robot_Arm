#include "servo_uart.h"
#include "usart.h"
#include "usbd_cdc_if.h"

#define RX_BUFFER_SIZE 512

uint8_t servo_rx_buffer[RX_BUFFER_SIZE] __attribute__((section(".ARM.__at_0x24000000")));

void servo_receive_init(void)
{
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2, servo_rx_buffer, RX_BUFFER_SIZE);
}

void USART2_IRQHandler(void)
{
    uint32_t temp = 0, receive_len = 0;
    
    if ((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        HAL_UART_DMAStop(&huart2);
        
        temp = __HAL_DMA_GET_COUNTER(huart2.hdmarx);
        receive_len = RX_BUFFER_SIZE - temp;
        
        /*******************************************/
        CDC_Transmit_FS(servo_rx_buffer, receive_len);  //test
        /*******************************************/
    }
    HAL_UART_Receive_DMA(&huart2, servo_rx_buffer, RX_BUFFER_SIZE);
    HAL_UART_IRQHandler(&huart2);
}


