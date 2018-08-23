#include "cmsis_os.h"
#include "task_state.h"
#include "application.h"
#include "console.h"
#include "uart-core.h"
#include "test-core.h"
#include "A9500_iot_module.h"




void soc_start()
{
	
	uart_core_task_create();
	create_state_task();
	console_task_create();
	ble_task_create();
#if TEST_FUNC
	test_func_start();
#endif

}


void bsp_module_start()
{
	ble_module_start();
	a9500_module_start();
}

void application_start()
{
	soc_start();
	bsp_module_start();
}
