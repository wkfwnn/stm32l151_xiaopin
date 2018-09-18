#include "motor_module.h"
#include "cmsis_os.h"
#include "stm32l1xx_hal.h"
#include "stdio.h"
#include "infrared_detection_module.h"
#include "user_define.h"



/*motor gpio control*/
#define MOTOR_GPIO_GROUP             GPIOB

#define MOTOR_GPIO_PIN1            GPIO_PIN_6 
#define MOTOR_GPIO_PIN2            GPIO_PIN_7





void motor_stop()
{
	HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN2,GPIO_PIN_SET);
}
void motor_module_start(void)
{
	motor_stop();
}

uint8_t  set_motor_module_status(uint8_t current_status,uint8_t status)
{
	uint16_t time_out = 0x00;
	uint8_t ret = RET_OK;
	switch (status){
		
	case MOTOR_MODULE_STATUS_FLAT_0_DEGREE:
		//always back forward
			HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN2,GPIO_PIN_SET);
			
			do{
				vTaskDelay(5);
				get_infrared_detection_status(&current_status);
				time_out += 5;
				printf("time out %d\n",time_out);
			
			}while((current_status != MOTOR_MODULE_STATUS_FLAT_0_DEGREE) && (time_out <= 20000));
				
			if(current_status == MOTOR_MODULE_STATUS_FLAT_0_DEGREE){
				ret = RET_OK;
			}else {
				ret = RET_TIME_OUT;
			}
			motor_stop();
			
		break;
	case MOTOR_MODULE_STATUS_OBLIQUE_0_90_DEGREE:
		
		break;
	case MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE:
		if(current_status == MOTOR_MODULE_STATUS_FLAT_0_DEGREE || 
		  current_status == MOTOR_MODULE_STATUS_OBLIQUE_0_90_DEGREE){
			//control motor forward
			HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN1,GPIO_PIN_SET);
			HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN2,GPIO_PIN_RESET);
				
			do{
				vTaskDelay(5);
				get_infrared_detection_status(&current_status);
				time_out += 5;
				printf("time out %d\n",time_out);
			
			}while((current_status != MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE) && (time_out <= 10000));
				
			if(current_status == MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE){
				ret = RET_OK;
			}else {
				ret = RET_TIME_OUT;
			}
			motor_stop();
			
		}else if(current_status == MOTOR_MODULE_STATUS_MORE_THAN_90_DEGREE){
			//control motor backward
			//control motor forward
			HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MOTOR_GPIO_GROUP,MOTOR_GPIO_PIN2,GPIO_PIN_SET);
			
			do{
				vTaskDelay(5);
				get_infrared_detection_status(&current_status);
				time_out += 5;
				printf("time out %d\n",time_out);
			
			}while((current_status != MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE) && (time_out <= 20000));
				
			if(current_status == MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE){
				ret = RET_OK;
			}else {
				ret = RET_TIME_OUT;
			}
			motor_stop();
			
			
		}
		break;
	case MOTOR_MODULE_STATUS_MORE_THAN_90_DEGREE:
		break;
	default:printf("set status is not support\n");
		break;
	}

	return ret;
}


void get_motor_module_status(uint8_t *status)
{
	



}


