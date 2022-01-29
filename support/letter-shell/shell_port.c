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


Shell shell;
char shellBuffer[512];

osThreadId shell_task_handler;

static SemaphoreHandle_t shellMutex;

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
    HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 0X1FF);
    
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
    if(HAL_UART_Receive(&huart1, (uint8_t *)data, len, 0xFFFF) != HAL_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }
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
    
    osThreadDef(shell_task, shellTask, osPriorityIdle, 0, 256);
    shell_task_handler = osThreadCreate(osThread(shell_task), &shell);
    
//    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS)
//    {
//        //logError("shell task creat failed");
//    }
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

