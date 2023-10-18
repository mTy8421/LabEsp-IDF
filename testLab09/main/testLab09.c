#include <stdio.h>
// #include "esp_idf_lib_helpers.h"

#include "driver/gpio.h"
#include "esp_log.h"
// #include "gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "rtos_example";

static void producer_task(void *arg) {
  // กำหนดค่า GPIO หมายเลข 0 เป็นอินพุต
  esp_rom_gpio_pad_select_gpio(GPIO_NUM_0);
  gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);

  // กำหนดตัวแปรเก็บค่า
  int value = 0;

  // วนลูปเพื่อรับสัญญาณจาก GPIO
  while (true) {
    // ตรวจสอบสถานะ GPIO
    int button_state = gpio_get_level(GPIO_NUM_0);

    // ถ้าปุ่มถูกกด
    if (button_state == 0) {
      // เพิ่มค่าตัวแปร
      value++;
    }

    // ถ้าปุ่มถูกปล่อย
    else {
      // ส่งค่าไปให้กับ Task consumer
      xTaskNotifyGive(consumer_task);

      // clear ค่ากลับ
      value = 0;
    }

    // รอรอบถัดไป
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

static void consumer_task(void *arg) {
  // วนลูปเพื่อรับค่าจาก Task producer
  while (true) {
    // รับค่าจาก Task producer
    int value;
    xTaskNotifyWait(0, 0, &value, portMAX_DELAY);

    // แสดงค่าที่รับได้
    ESP_LOGI(TAG, "Value: %d", value);
  }
}

void app_main(void) {
  // กำหนดลำดับความสำคัญของ Task producer
  xTaskCreate(producer_task, "producer_task", 1024, NULL, 1, &producer_task);

  // กำหนดลำดับความสำคัญของ Task consumer
  xTaskCreate(consumer_task, "consumer_task", 1024, NULL, 1, &consumer_task);

  // รอจนกว่าระบบปฏิบัติการจะทำงานเสร็จ
  vTaskStartScheduler();
}
