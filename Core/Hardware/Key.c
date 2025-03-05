#include "gpio.h"
/**
 * 函    数：按键初始化
 * 参    数：无
 * 返 回 值：无
 */
/**
 * @description: 按钮初始化
 * @return {*}
 */
void Key_Init(void)
{
	MX_GPIO_Init();
}

/**
 * 函    数：按键获取键码
 * 参    数：无
 * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
 * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
 */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0; // 定义变量，默认键码值为0

	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) // 读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		vTaskDelay(500); // 延时消抖
		while (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET);		   // 等待按键松手
		vTaskDelay(500); // 延时消抖
		KeyNum = 1;	   // 置键码为2
	}

	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET) // 读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		vTaskDelay(500); // 延时消抖
		while (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET);		   // 等待按键松手
		vTaskDelay(500); // 延时消抖
		KeyNum = 2;	   // 置键码为2
	}

	return KeyNum; // 返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
