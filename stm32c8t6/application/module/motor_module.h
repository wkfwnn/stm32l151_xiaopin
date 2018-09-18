#ifndef __MOTOR_MODULE_H_
#define __MOTOR_MODULE_H_

#include "cmsis_os.h"




uint8_t set_motor_module_status(uint8_t current_status,uint8_t status);

void motor_module_start(void);


#endif
