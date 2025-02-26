#include "FreeRTOS.h"
#include "task.h"
#include "freertos_demo.h"
#include "LED.h"
#include "gpio.h"
#include "OLED.h"
#include "MPU6050.h"
#include "key.h"

/* 启动任务函数 */
#define START_TASK_PRIORITY 1
#define START_TASK_STACK_DEPTH 64
TaskHandle_t start_task_handler;
void Start_Task(void *pvParameters);

/* LED 任务 配置 */
#define TASK1_PRIORITY 3
#define TASK1_STACK_DEPTH 36
TaskHandle_t task1_handler;
void TaskLedOn(void *pvParameters);

/* MPU6050 任务 配置 */
#define TASK2_PRIORITY 2
#define TASK2_STACK_DEPTH 128
TaskHandle_t task2_handler;
void MPU6050(void *pvParameters);

/* KEY 任务 配置 */
#define TASK3_PRIORITY 3
#define TASK3_STACK_DEPTH 36
TaskHandle_t task3_handler;
void KEY(void *pvParameters);

void FreeRTOS_Start(void)
{
    xTaskCreate((TaskFunction_t)Start_Task,
                (char *)"Start_Task",
                (configSTACK_DEPTH_TYPE)START_TASK_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIORITY,
                (TaskHandle_t *)&start_task_handler);

    vTaskStartScheduler(); /* 启动任务调度 */
}

void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */
    xTaskCreate((TaskFunction_t)TaskLedOn,
                (char *)"TaskLedOn",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&task1_handler);
    xTaskCreate((TaskFunction_t)MPU6050,
                (char *)"MPU6050",
                (configSTACK_DEPTH_TYPE)TASK2_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK2_PRIORITY,
                (TaskHandle_t *)&task2_handler);
    xTaskCreate((TaskFunction_t)KEY,
                (char *)"KEY",
                (configSTACK_DEPTH_TYPE)TASK3_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY,
                (TaskHandle_t *)&task3_handler);

    taskEXIT_CRITICAL(); /* 退出临界区 */
    vTaskDelete(NULL);   /* 任务自删除 */
}

/**
 * @description: LED1每500ms翻转一次
 * @param {void *} pvParameters
 * @return {*}
 */
void TaskLedOn(void *pvParameters)
{
    while (1)
    {
        LED_ON_OFF(1);
    }
}

void MPU6050(void *pvParameters)
{
    uint8_t ID;                     // 定义用于存放ID号的变量
    int16_t AX, AY, AZ, GX, GY, GZ; // 定义用于存放各个数据的变量

    OLED_Init();    // OLED初始化
    MPU6050_Init(); // MPU6050初始化

    OLED_ShowString(1, 1, "ID:"); // 显示静态字符串
    ID = MPU6050_GetID();         // 获取MPU6050的ID号
    OLED_ShowHexNum(1, 4, ID, 2); // OLED显示ID号

    while (1)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据
        OLED_ShowSignedNum(2, 1, AX, 5);               // OLED显示数据
        OLED_ShowSignedNum(3, 1, AY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
    }
}

void KEY(void *pvParameters)
{
    //LED_ON(2);
    while (1)
    {
        int KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            vTaskResume(task1_handler); //任务恢复
        }
        else if (KeyNum == 2)
        {
            vTaskSuspend(task1_handler); //任务挂起
        }
    }
}
