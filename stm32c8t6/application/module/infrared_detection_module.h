#ifndef __INFRARED_DETECTION_H_
#define __INFRARED_DETECTION_H_

#include "cmsis_os.h"


//this enum is 
typedef enum {

	MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE = 0,	
	MOTOR_MODULE_STATUS_OBLIQUE_0_90_DEGREE = 1,	
	MOTOR_MODULE_STATUS_MORE_THAN_90_DEGREE = 2,
	MOTOR_MODULE_STATUS_FLAT_0_DEGREE = 3,
	
}motor_status_t;


typedef void (*infrared_status_change)(void);

void infrared_detection_module_start(void);
void get_infrared_detection_status(uint8_t *status);
int register_infrared_detection_status_call_back(infrared_status_change call_back);

#endif
