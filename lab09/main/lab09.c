#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define STACK_SIZE 2048
#define TASK_PRIORITY 10

const char *pcTaskForTask1 = "Task 1 is running\r\n";
const char *pcTaskForTask2 = "Task 2 is running\r\n";

void vTaskFunction(void *pvParameters)
{
    while (1)
    {
        printf("%s", (char *)pvParameters);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(vTaskFunction,
                "Task 1",
                STACK_SIZE,
                (void *)pcTaskForTask1,
                TASK_PRIORITY,
                NULL
        );
    
    xTaskCreate(vTaskFunction,
                "Task 2",
                STACK_SIZE,
                (void *)pcTaskForTask2,
                TASK_PRIORITY,
                NULL
        );
    
    while (1)
    {
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}
