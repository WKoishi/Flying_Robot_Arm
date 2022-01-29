/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "cmsis_os.h"
#include "semphr.h"
#include "shell.h"
//#include "serial.h"
#include "stm32h7xx_hal.h"
#include "usart.h"
//#include "cevent.h"
//#include "log.h"

#define shell_huart huart1

Shell shell;
char shellBuffer[512];

osThreadId shell_task_handler;

static SemaphoreHandle_t shellMutex;

typedef struct {
    char data_temp;
    uint8_t flag;
} ShellReceiver;
static ShellReceiver receiver;

/**
 * @brief USART1 interrupt function
 */
void USART1_IRQHandler(void)
{
    if ((__HAL_UART_GET_FLAG(&shell_huart, UART_FLAG_RXNE) != RESET))
    {
        receiver.data_temp = shell_huart.Instance->RDR;
        receiver.flag = 1;
        osSignalSet(shell_task_handler, 1);
    }
    HAL_UART_IRQHandler(&shell_huart);
}

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    //serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
    HAL_UART_Transmit(&shell_huart, (uint8_t*)data, len, 0X1FF);
    
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    short ret_val = 0;
    
    if (0 == receiver.flag)
        osSignalWait(1, portMAX_DELAY);
    
    if (receiver.flag)
    {
        *data = receiver.data_temp;
        receiver.flag = 0;
        ret_val = 1;
    }
    
    return ret_val;
    //return serialReceive(&debugSerial, (uint8_t *)data, len, 0);
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    xSemaphoreTake(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    xSemaphoreGive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
    
    __HAL_UART_ENABLE_IT(&shell_huart, UART_IT_RXNE);
    
    osThreadDef(shell_task, shellTask, osPriorityHigh, 0, 256);
    shell_task_handler = osThreadCreate(osThread(shell_task), &shell);
    
//    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS)
//    {
//        //logError("shell task creat failed");
//    }
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

