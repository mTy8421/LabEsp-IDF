#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

uint32_t ulVar = 0;
QueueHandle_t queue;
uint32_t LOW = 0;
#define BUTTON GPIO_NUM_0

int num = 0;

int gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
int gpio_set_level(BUTTON, LOW);

void taskProducer(void *pvParameter)
{
  while (1)
  {
    if (gpio_get_level(BUTTON) == 0)
    {
      ulVar++;
      printf("num = %ld\r\n",ulVar);
      num = 1;
    }
    else
    {
      xQueueSend(queue, (void *)&ulVar, (TickType_t)10);
      ulVar = 0;
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void taskConsumer(void *pvParameter)
{
  uint32_t data;
  while (1)
  {
    if (gpio_get_level(BUTTON) == 1)
    {
      if (xQueueReceive(queue, &data, portMAX_DELAY) && num != 0)
      {
        printf("[Queue] Recv, val: %ld\r\n", data);
        num = 0;
      }
    }
  }
  vTaskDelete(NULL);
}

void app_main(void)
{
  queue = xQueueCreate(10, sizeof(uint32_t));

  xTaskCreate(taskProducer, "Producer", 2048, NULL, 10, NULL);
  xTaskCreate(taskConsumer, "Consumer", 2048, NULL, 10, NULL);

  while (1)
  {

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}