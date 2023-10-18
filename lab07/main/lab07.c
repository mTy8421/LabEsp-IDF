#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

static const char *TAG = "MCPWM";

#define SERVO_MIN_PULSEWIDTH_US 500
#define SERVO_MAX_PULSEWIDTH_US 2500
#define SERVO_MIN_DEGREE -90
#define SERVO_MAX_DEGREE 90

#define SERVO_PULSE_GPIO 	13
#define SEROVO_TIMEBASE_RESOLUTION_HZ 1000000
#define SEROVO_TIMEBASE_PERIOD 20000

static inline uint32_t example_angle_to_compare(int angle)
{
	return(angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US)/(SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

void app_main(void)
{
	ESP_LOGI(TAG, "Create timer and operator");
	mcpwm_timer_handle_t timer = NULL;
	mcpwm_timer_config_t timer_config = {
		.group_id = 0,
		.clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
		.resolution_hz = SEROVO_TIMEBASE_RESOLUTION_HZ,
		.period_ticks = SEROVO_TIMEBASE_PERIOD,
		.count_mode = MCPWM_TIMER_COUNT_MODE_UP,
	};
	ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config,&timer));

	mcpwm_oper_handle_t oper = NULL;
	mcpwm_operator_config_t operator_config = {
		.group_id = 0,
	};
	ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config,&oper));

	ESP_LOGI(TAG, "Connect timer and operator");
	ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper,timer));

	ESP_LOGI(TAG, "Create comparator and generator from the operator");
	mcpwm_cmpr_handle_t comparator = NULL;
	mcpwm_comparator_config_t comparator_config = {
		.flags.update_cmp_on_tez = true,
	};
	ESP_ERROR_CHECK(mcpwm_new_comparator(oper,&comparator_config,&comparator));
	mcpwm_gen_handle_t generator = NULL;
	mcpwm_generator_config_t generator_config = {
		.gen_gpio_num = SERVO_PULSE_GPIO,
	};
	ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

	ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator,example_angle_to_compare(0)));
	
	ESP_LOGI(TAG, "Set generator action on timer and compare event");
	
	ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
			MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));

	ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
			MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator , MCPWM_GEN_ACTION_LOW)));

	ESP_LOGI(TAG, "Enable and start timer");
	ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
	ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer,MCPWM_TIMER_START_NO_STOP));
	
	int angle = 0;
	int step =2;
	while(1){
		ESP_LOGI(TAG, "Angle of rotation: %d",angle);
		ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator,example_angle_to_compare(angle)));
	
	vTaskDelay(pdMS_TO_TICKS(500));
	if((angle + step) > 60 || (angle + step) <-60){
		step *=-1;
	}
	angle += step;
}
}