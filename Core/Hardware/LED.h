#ifndef LED_H
#define LED_H

#include "gpio.h"

void LED_ON(uint8_t LEDx);     // 打开指定LED
void LED_OFF(uint8_t LEDx);    // 关闭指定LED
void LED_Toggle(uint8_t LEDx); // 翻转指定LED状态
void LED_ON_OFF(uint8_t LEDx); // 打开指定LED，关闭其他LED

#endif
