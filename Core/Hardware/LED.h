/*
 * @Author: 3248698481 onmylive842@gmail.com
 * @Date: 2025-02-17 15:52:08
 * @LastEditors: 3248698481 onmylive842@gmail.com
 * @LastEditTime: 2025-02-28 14:47:57
 * @FilePath: \MDK-ARMe:\Personal\HalStm32\01.LED\Core\Hardware\LED.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef LED_H
#define LED_H

#include "gpio.h"

void LED_ON(uint8_t LEDx);     // 打开指定LED
void LED_OFF(uint8_t LEDx);    // 关闭指定LED
void LED_Toggle(uint8_t LEDx); // 翻转指定LED状态
void LED_ON_OFF();             // 打开指定LED，关闭其他LED

#endif
