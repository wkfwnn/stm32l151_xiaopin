#ifndef _A9500_IOT_H_
#define _A9500_IOT_H_


#define A9500MGGT_GPIO_GROUP       GPIOA

#define A9500MGGT_PIN5_POWER_ON    GPIO_PIN_5
#define A9500MGGT_PIN9_UART1_DTR   GPIO_PIN_1
#define A9500MGGT_PIN12_UART1_DSR  GPIO_PIN_0
#define A9500MGGT_PIN13_VOUT_1V8   GPIO_PIN_6
#define A9500MGGT_PIN80_RESET      GPIO_PIN_4




/*software function*/
void a9500_module_start();


#endif