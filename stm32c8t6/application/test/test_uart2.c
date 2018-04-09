#include "test_uart2.h"

#if TEST_UART2_FUNC

void test_uart2_function(void const * argument)
{
	(void)(argument);
	uart_handle_t test_uart2_handle;
	user_error_t sc;
	sc = uart_get_handle("uart2",sizeof("uart2"),&test_uart2_handle);
	if(sc == RET_OK){
		printf("test_uart2 get %s handle is %d\n","uart2",test_uart2_handle);
	}else{
		printf("test_uart2 get %s handle fail %d,exit test_uart1_task \n","uart2",sc);
		vTaskDelete(NULL);
	}
	while(1){
		//sc = uart_write_data(test_uart2_handle, "this from uart2 test func hello\n", 
					//	sizeof("this from uart2 test func hello\n"));
		//if(sc == RET_OK){
		//	printf("test_uart2 transmit ok\n");
		//}else{
		//	printf("uart2 send fail %d\n",sc);
		//}
		vTaskDelay(1000);
	}
}

void test_uart2_func()
{
	osThreadId test_uart2_handle;
	osThreadDef(test_uart2, test_uart2_function, osPriorityLow, 0, 128);
	test_uart2_handle = osThreadCreate(osThread(test_uart2), NULL);
	if(test_uart2_handle == NULL){
		printf("test_uart2_function create fail\n");
	}else{
		printf("test_uart2_function create success\n");
	}
}


#endif

