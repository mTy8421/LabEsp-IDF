#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON GPIO_NUM_0
#define LED GPIO_NUM_2

static uint32_t toggle = 0;

static uint32_t LOW = 0;

static void config(void)
{
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    gpio_set_level(BUTTON, LOW);
}

static void led(void)
{
    gpio_set_level(LED, toggle);
}

void app_main(void)
{
    config();

    while (true)
    {
        led();
        if (gpio_get_level(BUTTON) == 0)
        {
            toggle = !toggle;
        }
        printf("%d\r\n", gpio_get_level(BUTTON));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
