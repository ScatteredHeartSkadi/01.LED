#include "FreeRTOS.h"
#include "task.h"
#include "freertos_demo.h"
#include "LED.h"
#include "gpio.h"
#include "OLED.h"
#include "MPU6050.h"
#include "key.h"
#include "usart.h"
#include "queue.h"

extern UART_HandleTypeDef huart1;

uint8_t arr[100];
uint16_t size = 0;
uint8_t isOver = 0;

/* 启动任务函数 */
#define START_TASK_PRIORITY 1
#define START_TASK_STACK_DEPTH 128
TaskHandle_t start_task_handler;
void Start_Task(void *pvParameters);

/* LED 任务 配置 */
#define TASK1_PRIORITY 3
#define TASK1_STACK_DEPTH 128
TaskHandle_t task1_handler;
void TaskLedOn(void *pvParameters);

// /* MPU6050 任务 配置 */
// #define TASK2_PRIORITY 2
// #define TASK2_STACK_DEPTH 128
// TaskHandle_t task2_handler;
// void MPU6050(void *pvParameters);

/* KEY 任务 配置 */
#define TASK3_PRIORITY 2
#define TASK3_STACK_DEPTH 128
TaskHandle_t task3_handler;
void KEY(void *pvParameters);

/* UARTEx 任务 配置 */
#define TASK4_PRIORITY 2
#define TASK4_STACK_DEPTH 128
TaskHandle_t task4_handler;
void UARTEx(void *pvParameters);

QueueHandle_t xQueue;

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
    // xTaskCreate((TaskFunction_t)MPU6050,
    //             (char *)"MPU6050",
    //             (configSTACK_DEPTH_TYPE)TASK2_STACK_DEPTH,
    //             (void *)NULL,
    //             (UBaseType_t)TASK2_PRIORITY,
    //             (TaskHandle_t *)&task2_handler);
    xTaskCreate((TaskFunction_t)KEY,
                (char *)"KEY",
                (configSTACK_DEPTH_TYPE)TASK3_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY,
                (TaskHandle_t *)&task3_handler);
    xTaskCreate((TaskFunction_t)UARTEx,
                (char *)"UARTEx",
                (configSTACK_DEPTH_TYPE)TASK4_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK4_PRIORITY,
                (TaskHandle_t *)&task4_handler);

    vTaskDelete(NULL);   /* 任务自删除 */
    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/*
 * 函    数：TaskLedOn
 * 功    能：任务LED亮的入口函数
 * 参    数：无
 * 返 回 值：无
 * 注意事项：此函数是阻塞式操作，会无限循环执行
 * 说明描述：管理LED亮的任务
 */
void TaskLedOn(void *pvParameters)
{
    while (1)
    {
        printf("LED");
        LED_ON_OFF();
    }
}

// void MPU6050(void *pvParameters)
// {
//     uint8_t ID;                     // 定义用于存放ID号的变量
//     int16_t AX, AY, AZ, GX, GY, GZ; // 定义用于存放各个数据的变量

//     OLED_Init();    // OLED初始化
//     MPU6050_Init(); // MPU6050初始化

//     OLED_ShowString(1, 1, "ID:"); // 显示静态字符串
//     ID = MPU6050_GetID();         // 获取MPU6050的ID号
//     OLED_ShowHexNum(1, 4, ID, 2); // OLED显示ID号

//     while (1)
//     {
//         MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据
//         OLED_ShowSignedNum(2, 1, AX, 5);               // OLED显示数据
//         OLED_ShowSignedNum(3, 1, AY, 5);
//         OLED_ShowSignedNum(4, 1, AZ, 5);
//         OLED_ShowSignedNum(2, 8, GX, 5);
//         OLED_ShowSignedNum(3, 8, GY, 5);
//         OLED_ShowSignedNum(4, 8, GZ, 5);
//     }
// }

/*
 * 函    数：KEY
 * 功    能：任务入口函数，用于处理按键事件
 * 参    数：pvParameters - 任务参数指针
 * 返 回 值：无
 * 注意事项：此函数会无限循环等待按键事件
 * 说明描述：根据获取到的按键编号，决定恢复或挂起task1_handler任务
 */
void KEY(void *pvParameters)
{
    printf("KEY");
    while (1)
    {
        int KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            taskENTER_CRITICAL(); // 进入临界区
            printf("1");
            vTaskDelay(20);
            vTaskResume(task1_handler); // 任务恢复
            taskEXIT_CRITICAL();        // 退出临界区
        }
        else if (KeyNum == 2)
        {
            taskENTER_CRITICAL(); // 进入临界区
            printf("2");
            vTaskDelay(20);
            vTaskSuspend(task1_handler); // 任务挂起
            taskEXIT_CRITICAL();         // 退出临界区
        }
    }
}

/*
 * 函    数：UARTEx
 * 功    能：处理UART接收和发送的任务
 * 参    数：pvParameters - 任务参数，无实际使用
 * 返 回 值：无
 * 注意事项：此函数在一个无限循环中运行，持续监听UART接收并根据状态发送数据
 * 说明描述：通过中断接收UART数据，当接收到空闲状态时，如果isOver标志被设置，则发送数据并重置标志
 */
void UARTEx(void *pvParameters)
{
    while (1)
    {
        // printf("hello world\n");
        // vTaskDelay(1000);
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, arr, 100);
        if (isOver)
        {
            HAL_UART_Transmit(&huart1, arr, size, 1000);
            isOver = 0;
        }
    }
}
