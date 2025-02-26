/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-02-17 15:52:08
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-25 22:54:44
 * @FilePath: \MDK-ARMe:\Personal\HalStm32\01.LED\Core\Src\majormain.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "main.h"
#include "freertos_demo.h"
#include "usart.h"

uint8_t arr[100];
uint16_t size = 0;
uint8_t isOver = 0;

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  // MPU6050_Init();
  // OLED_I2C_Init();
  MX_I2C2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();

  // FreeRTOS_Start();
  // Start_Task(NULL);
  // TaskLedOn(NULL);
  // MPU6050(NULL);
  // KEY(NULL);

  // printf("hello world\n");
  while (1)
  {
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, arr, 100);
    if (isOver)
    {
      HAL_UART_Transmit(&huart1, arr, size, HAL_MAX_DELAY);
      isOver = 0;
    }
  }
}
