/*
 * @Author: 3248698481 onmylive842@gmail.com
 * @Date: 2025-02-17 15:52:08
 * @LastEditors: 3248698481 onmylive842@gmail.com
 * @LastEditTime: 2025-03-01 13:02:41
 * @FilePath: \MDK-ARMe:\Personal\HalStm32\01.LED\Core\Hardware\LED.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "LED.h"

// ...existing code...

void LED_ON(uint8_t LEDx)
{
   // 实现打开指定LED的逻辑
   switch (LEDx)
   {
   case 1:
      HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_RESET);
      break;
   case 2:
      HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_RESET);
      break;
   case 3:
      HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_RESET);
      break;
   default:
      break;
   }
}

void LED_OFF(uint8_t LEDx)
{
   // 实现关闭指定LED的逻辑
   switch (LEDx)
   {
   case 1:
      HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_SET);
      break;
   case 2:
      HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_SET);
      break;
   case 3:
      HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_SET);
      break;
   default:
      break;
   }
}

void LED_Toggle(uint8_t LEDx)
{
   // 实现翻转指定LED状态的逻辑
   switch (LEDx)
   {
   case 1:
      HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
      break;
   case 2:
      HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
      break;
   case 3:
      HAL_GPIO_TogglePin(GPIOA, LED3_Pin);
      break;
   default:
      break;
   }
}

/// @brief LED灯流转
/// @param LEDx
void LED_ON_OFF()
{
   for (uint8_t i = 1; i <= 3; i++)
   {
      LED_ON(i);
      vTaskDelay(500);
      LED_OFF(i);
      vTaskDelay(500);
   }
}

// ...existing code...
