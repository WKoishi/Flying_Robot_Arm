#include "usb_uart.h"
#include "stdarg.h"
#include "usbd_cdc_if.h"

extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

void usb_printf(const char *format, ...)
{
    va_list args;
    uint32_t length = 0;
 
    va_start(args, format);
    length = vsnprintf((char *)UserTxBufferFS, APP_TX_DATA_SIZE, (char *)format, args);
    va_end(args);
    CDC_Transmit_FS(UserTxBufferFS, length);
}


