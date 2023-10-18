#include <stdio.h>
#include <sys/time.h>
#include "esp_sleep.h"

static RTC_DATA_ATTR struct timeval sleep_enter_timer;

RTC_DATA_ATTR int Num = 0;

void app_main(void)
{
    
    const int wakeup_time_sec = 5;
    printf("Enabling timer wakeup, %ds\r\n", wakeup_time_sec);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000 *1000));

    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_timer.tv_sec) * 1000 + (now.tv_usec - sleep_enter_timer.tv_usec) / 1000;

    switch (esp_sleep_get_wakeup_cause())
    {
    case ESP_SLEEP_WAKEUP_TIMER:
        printf("Wake up from timer. Time spent in deep sleep: %dms\r\n", sleep_time_ms);
        Num++;
        printf("Number sleep: %d\r\n",Num);
        break;
    
    default:
        printf("Not a deep sleep reset\r\n");
        break;
    }

    printf("Entering deep sleep\r\n");
    gettimeofday(&sleep_enter_timer,NULL);

    esp_deep_sleep_start();
}
