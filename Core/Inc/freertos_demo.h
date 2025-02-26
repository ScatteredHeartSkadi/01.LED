#ifndef __FREERTOS_DEMO_H
#define __FREERTOS_DEMO_H

void FreeRTOS_Start(void);
void Start_Task(void *pvParameters);
void TaskLedOn(void *pvParameters);
void MPU6050(void *pvParameters);
void KEY(void *pvParameters);

#endif /* __FREERTOS_DEMO_H */
