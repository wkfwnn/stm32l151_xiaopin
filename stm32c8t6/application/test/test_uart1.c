#include "test_uart1.h"


#if TEST_UART1_FUNC

void test_uart1_function(void const * argument)
{
	(void)(argument);
	uart_handle_t test_uart1_handle;
	user_error_t sc;
	sc = uart_get_handle("uart1",sizeof("uart1"),&test_uart1_handle);
	if(sc == RET_OK){
		DBG_LOG("test_uart1 get %s handle is %d\n","uart1",test_uart1_handle);
	}else{
		DBG_LOG("test_uart1 get %s handle fail %d,exit test_uart1_task \n","uart1",sc);
		vTaskDelete(NULL);
	}
	while(1){
		sc = uart_write_data(test_uart1_handle, "this from uart1 test func hello\n", 
						sizeof("this from uart1 test func hello\n"));
		if(sc == RET_OK){
			DBG_LOG("test_uart1 transmit ok\n");
		}
		vTaskDelay(1000);
	}
}

void test_uart11_function(void const * argument)
{
	(void)(argument);
	uart_handle_t test_uart11_handle;
	user_error_t sc;
	sc = uart_get_handle("uart1",sizeof("uart1"),&test_uart11_handle);
	if(sc == RET_OK){
		DBG_LOG("test_uart11 get %s handle is %d\n","uart1",test_uart11_handle);
	}else{
		DBG_LOG("test_uart11 get %s handle fail %d,exit test_uart1_task \n","uart1",sc);
		vTaskDelete(NULL);
	}
	while(1){
		sc = uart_write_data(test_uart11_handle, "this from uart11 test func hello\n", 
						sizeof("this from uart11 test func hello\n"));
		if(sc == RET_OK){
			DBG_LOG("test_uart11 transmit ok\n");
		}
		vTaskDelay(1000);
	}
}

void test_uart1_func()
{
	osThreadId test_uart1_handle;
	osThreadDef(test_uart1, test_uart1_function, osPriorityLow, 0, 128);
	test_uart1_handle = osThreadCreate(osThread(test_uart1), NULL);
	if(test_uart1_handle == NULL){
		DBG_LOG("test_uart1_function create fail\n");
	}else{
		DBG_LOG("test_uart1_function create success\n");
	}
	
	osThreadId test_uart11_handle;
	osThreadDef(test_uart11, test_uart11_function, osPriorityLow, 0, 128);
	test_uart11_handle = osThreadCreate(osThread(test_uart11), NULL);
	if(test_uart11_handle == NULL){
		DBG_LOG("test_uart1_function create fail\n");
	}else{
		DBG_LOG("test_uart1_function create success\n");
	}
}








#endif

