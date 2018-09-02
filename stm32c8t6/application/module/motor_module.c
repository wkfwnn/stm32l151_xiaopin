#include "motor_module.h"

/*speak control*/
#define SPEAK_GPIO_GROUP             GPIOA
#define SPEAK_GPIO_CONTROL           GPIO_PIN_8

#define speak_alarm()  (HAL_GPIO_WritePin(SPEAK_GPIO_GROUP,SPEAK_GPIO_CONTROL ,GPIO_PIN_SET))
#define speak_mute()   (HAL_GPIO_WritePin(SPEAK_GPIO_GROUP,SPEAK_GPIO_CONTROL ,GPIO_PIN_RESET))




/*motor gpio control*/
#define MOTOR_GPIO_GROUP             GPIOB

#define MOTOR_GPIO_PIN1            GPIO_PIN_6 
#define MOTOR_GPIO_PIN2            GPIO_PIN_7



/*Photodiode status*/
#define PHOTODIODE_GPIO_GROUP        GPIOB
#define PHOTODIODE_S1                GPIO_PIN_0
#define PHOTODIODE_S2                GPIO_PIN_1




/*Infrared light */
#define INFRARED_GPIO_GROUP             GPIOB
#define INFRARED_GPIO_CONTROL           GPIO_PIN_5

#define enable_infrared_led()           (HAL_GPIO_WritePin(INFRARED_GPIO_GROUP,INFRARED_GPIO_CONTROL,GPIO_PIN_SET))
#define disable_infrared_led()          (HAL_GPIO_WritePin(INFRARED_GPIO_GROUP,INFRARED_GPIO_CONTROL,GPIO_PIN_RESET)) 

void motor_module_start(void)
{
	
}

void set_motor_module_status(uint8_t status)
{
	switch (status){
	case MOTOR_MODULE_STATUS_FLAT_0_DEGREE:
		break;
	case MOTOR_MODULE_STATUS_OBLIQUE_0_90_DEGREE:
		break;
	case MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE:
		break;
	case MOTOR_MODULE_STATUS_MORE_THAN_90_DEGREE:
		break;
	case MOTOR_MODULE_STATUS_FALL_BACK_0_90_DEGREE:
		break;
	default:printf("set status is not support\n");
		break;
	}
}


void get_motor_module_status(uint8_t *status)
{
	



}


void set_speak_out_put(uint8_t status)
{
	switch (status){
		case SPEAK_STATUS_ALARM:
			break;	
		case SPEAK_STATUS_MUTE:
			break;
		default:printf("set speak out put do not support\n");
		break;
	}

}


