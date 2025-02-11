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

void LED_ON_OFF(uint8_t LEDx)
{
   for (uint8_t i = 1; i <= 3; i++)
   {
      LED_ON(i);
      HAL_Delay(500);
      LED_OFF(i);
      HAL_Delay(500);
   }
}

// ...existing code...
