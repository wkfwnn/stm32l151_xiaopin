#ifndef __USER_GPIO_MODULE_H_
#define __USER_GPIO_MODULE_H_

#include "cmsis_os.h"

typedef void (*gpio_exti_callback)(uint16_t pin);

int register_gpio_exti_callback(gpio_exti_callback func);



#endif