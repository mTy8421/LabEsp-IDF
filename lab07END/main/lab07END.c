#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define LED_PIN GPIO_NUM_13

static void timer_callback(void* arg);

void app_main(){
    esp_rom_gpio_pad_select_gpio (LED_PIN);
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);

    gpio_set_level(LED_PIN,0);
    
    esp_timer_handle_t timer;
    const esp_timer_create_args_t timer_args ={
        .callback = &timer_callback,
        .arg = NULL,
        .name = "led_timer"
    };
    esp_timer_create(&timer_args,&timer);

    esp_timer_start_periodic(timer,250000);
}

static void timer_callback(void* arg){
    static bool led_state = false;
    led_state = !led_state;
    gpio_set_level(LED_PIN, led_state);
}