#include "A9500_iot_module.h"
#include "A9500_module_at_commond.h"

void a9500mggt_reset()
{
	HAL_GPIO_WritePin(A9500MGGT_GPIO_GROUP,A9500MGGT_PIN80_RESET ,GPIO_PIN_RESET);	
	osDelay (200);	
	HAL_GPIO_WritePin(A9500MGGT_GPIO_GROUP,A9500MGGT_PIN80_RESET ,GPIO_PIN_SET);	
}

/*为了保证模块能够正常启动，在POWER_ON#动作后的 15 秒内请勿将 RESET 拉低*/
int a9500mggt_power_on_and_check()
{
	int sc = RET_OK;
	
	HAL_GPIO_WritePin(A9500MGGT_GPIO_GROUP,A9500MGGT_PIN5_POWER_ON ,GPIO_PIN_RESET);
	osDelay (1000);
	HAL_GPIO_WritePin(A9500MGGT_GPIO_GROUP,A9500MGGT_PIN5_POWER_ON ,GPIO_PIN_SET);
	osDelay (100);
	GPIO_PinState pin_state = HAL_GPIO_ReadPin(A9500MGGT_GPIO_GROUP, A9500MGGT_PIN13_VOUT_1V8);
	if(pin_state == GPIO_PIN_SET){
		sc = RET_OK;
	}else{
		sc = RET_ERROR;
	}
}



static int a9500_self_test()
{
	int sc = RET_OK;
	sc = a9500mggt_power_on_and_check();
	if(sc != RET_OK){
		printf("a9500 module self test fail\n");
		goto return_value;
	}
	
return_value:
	return sc;
}


void a9500_module_start()
{
	a9500_self_test()	


}
