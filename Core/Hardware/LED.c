#include "LED.h"

void LED_ON(uint8_t LEDx)
{
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

void LED_OFF(uint8_t LEDx)
{
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

void LED_Toggle(uint8_t LEDx)
{
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
