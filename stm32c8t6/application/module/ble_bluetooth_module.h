#ifndef _BLE_BLUE_TOOTH_MODULE_H_
#define _BLE_BLUE_TOOTH_MODULE_H_


#include "stm32l1xx_hal.h"
#include "uart-core.h"

/*hardware config*/
#define BLE_GPIO_GROUP             GPIOB 
#define BLE_MODULE_RTS_P0_0        GPIO_PIN_13    
#define BLE_MODULE_CMD_P0_1        GPIO_PIN_14
#define BLE_MODULE_SET_P0_4        GPIO_PIN_15 
#define BLE_MODULE_CTS_P0_6        ()    
#define BLE_MODULE_CONN_P1_0       ()
#define BLE_MODULE_SLEEP_P2_0      GPIO_PIN_12

#define BLE_UART              "uart3"


/*software define */

#define BLE_MAX_CALL_BACK_COUNT    1
typedef void (*ble_read_call_back)(uint8_t *data,uint16_t size);





typedef struct {
	uart_handle_t handle;
	uint8_t ble_call_back_count;
	ble_read_call_back ble_call_back_map[BLE_MAX_CALL_BACK_COUNT];
	
}ble_module_private_data_t;



/*external infterface */
void ble_module_start(void);
int ble_data_read_call_back_register(ble_read_call_back call_back);

#endif
