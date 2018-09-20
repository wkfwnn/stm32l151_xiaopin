#include "infrared_detection_module.h"
#include "cmsis_os.h"
#include "stm32l1xx_hal.h"
#include "user_define.h"
#include "user_gpio_module.h"



/*Photodiode status*/
#define PHOTODIODE_GPIO_GROUP        GPIOB
#define PHOTODIODE_S1                GPIO_PIN_0
#define PHOTODIODE_S2                GPIO_PIN_1

/*Infrared light */
#define INFRARED_GPIO_GROUP             GPIOB
#define INFRARED_GPIO_CONTROL           GPIO_PIN_5

#define enable_infrared_led()           (HAL_GPIO_WritePin(INFRARED_GPIO_GROUP,INFRARED_GPIO_CONTROL,GPIO_PIN_SET))
#define disable_infrared_led()          (HAL_GPIO_WritePin(INFRARED_GPIO_GROUP,INFRARED_GPIO_CONTROL,GPIO_PIN_RESET))

#define get_infrared_s1()               (HAL_GPIO_ReadPin(PHOTODIODE_GPIO_GROUP, PHOTODIODE_S1))
#define get_infrared_s2()               (HAL_GPIO_ReadPin(PHOTODIODE_GPIO_GROUP, PHOTODIODE_S2))

#define INFRARED_DETECTION_CALLBACK_ARRAY_SIZE  2
static infrared_status_change infrared_detection_callback_array[INFRARED_DETECTION_CALLBACK_ARRAY_SIZE] = {NULL,};
static uint8_t infrared_detection_callback_count = 0x00;


void gpio_exti_call_back(uint16_t pin)
{
	uint8_t i = 0;
	//DBG_LOG_ISR("gpio_exti_call_back pin %d\n",pin);
	for (i = 0; i < infrared_detection_callback_count; ++i){
		DBG_LOG_ISR("call  infrared_detection\n");
		infrared_detection_callback_array[i]();	
	}

}

void infrared_detection_module_start()
{
	int ret  = RET_OK;
	enable_infrared_led();
	
	ret = register_gpio_exti_callback(gpio_exti_call_back);
	if(ret != RET_OK){
		DBG_LOG("register_gpio_exti_callback fail\n");
	}
	
}

void infrared_detection_module_stop()
{
	disable_infrared_led();
}

void get_infrared_detection_status(uint8_t *status)
{
	*status = (get_infrared_s1() << 1)  + get_infrared_s2();	
}

int register_infrared_detection_status_call_back(infrared_status_change call_back)
{
	if(infrared_detection_callback_count < INFRARED_DETECTION_CALLBACK_ARRAY_SIZE){
		infrared_detection_callback_array[infrared_detection_callback_count++] = call_back;
		return RET_OK;
	}else{
		return RET_OVER_FLOW;
	}
	
}
