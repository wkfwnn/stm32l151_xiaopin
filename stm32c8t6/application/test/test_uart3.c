#include "test_uart3.h"
#if TEST_UART3_FUNC

void test_uart3_function(void const * argument)
{
	(void)(argument);
	uart_handle_t test_uart3_handle;
	user_error_t sc;
	sc = uart_get_handle("uart3",sizeof("uart3"),&test_uart3_handle);
	if(sc == RET_OK){
		printf("test_uart3 get %s handle is %d\n","uart3",test_uart3_handle);
	}else{
		printf("test_uart3 get %s handle fail %d,exit test_uart3_task \n","uart3",sc);
		vTaskDelete(NULL);
	}
	while(1){
		sc = uart_write_data(test_uart3_handle, "this from uart3 test func hello\n", 
						sizeof("this from uart3 test func hello\n"));
		if(sc == RET_OK){
			printf("test_uart3 transmit ok\n");
		}else{
			printf("uart3 send fail %d\n",sc);
		}
		vTaskDelay(1000);
	}
}

void test_uart3_func()
{
	osThreadId test_uart3_handle;
	osThreadDef(test_uart3, test_uart3_function, osPriorityLow, 0, 128);
	test_uart3_handle = osThreadCreate(osThread(test_uart3), NULL);
	if(test_uart3_handle == NULL){
		printf("test_uart3_function create fail\n");
	}else{
		printf("test_uart3_function create success\n");
	}
}
#endif

