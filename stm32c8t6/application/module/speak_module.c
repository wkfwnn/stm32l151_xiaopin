#include "speak_module.h"
#include "stm32l1xx_hal.h"
#include "user_define.h"


/*speak control*/
#define SPEAK_GPIO_GROUP             GPIOA
#define SPEAK_GPIO_CONTROL           GPIO_PIN_8

#define speak_alarm()  (HAL_GPIO_WritePin(SPEAK_GPIO_GROUP,SPEAK_GPIO_CONTROL ,GPIO_PIN_SET))
#define speak_mute()   (HAL_GPIO_WritePin(SPEAK_GPIO_GROUP,SPEAK_GPIO_CONTROL ,GPIO_PIN_RESET))



void set_speak_out_put(uint8_t status)
{
	switch (status){
		case SPEAK_STATUS_ALARM:speak_alarm();
			break;	
		case SPEAK_STATUS_MUTE:speak_mute();
			break;
		default:DBG_LOG("set speak out put do not support\n");
		break;
	}

}

