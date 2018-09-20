#include "cmsis_os.h"
#include "stdio.h"
#include "user_gpio_module.h"
#include "user_define.h"

#define GPIO_EXTI_CALLBACK_SIZE 16
gpio_exti_callback gpio_exti_array[GPIO_EXTI_CALLBACK_SIZE] = {NULL,};
uint8_t gpio_exti_callback_count = 0x00;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	uint8_t i = 0;
	//DBG_LOG_ISR("exti gpio pin %d\n",GPIO_Pin);
	for(i = 0;i < gpio_exti_callback_count;i++){
		gpio_exti_array[i](GPIO_Pin);
	}

}

int register_gpio_exti_callback(gpio_exti_callback func)
{

	if(gpio_exti_callback_count < GPIO_EXTI_CALLBACK_SIZE){
		gpio_exti_array[gpio_exti_callback_count++] = func;
		return RET_OK;
	}else{
		return RET_OVER_FLOW;
	}
}

