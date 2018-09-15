#ifndef __MOTOR_MODULE_H_
#define __MOTOR_MODULE_H_

#include "cmsis_os.h"

typedef enum {
	MOTOR_MODULE_STATUS_NONE,
		
	MOTOR_MODULE_STATUS_FLAT_0_DEGREE,
	MOTOR_MODULE_STATUS_OBLIQUE_0_90_DEGREE,
	MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE,
	MOTOR_MODULE_STATUS_MORE_THAN_90_DEGREE,
	MOTOR_MODULE_STATUS_FALL_BACK_0_90_DEGREE,

	
	MOTOR_MODULE_STATUS_END = 0x10,

}motor_status_t;



typedef enum {
	SPEAK_STATUS_NONE,
		
	SPEAK_STATUS_ALARM,
	SPEAK_STATUS_MUTE,
	
	SPEAK_STATUS_END
	
}speak_status_t;

void motor_module_start(void);
void set_speak_out_put(uint8_t status);

#endif
