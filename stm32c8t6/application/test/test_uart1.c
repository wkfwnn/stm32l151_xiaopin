#include "test_uart1.h"


#if TEST_UART1_FUNC

void test_uart1_function(void const * argument)
{
	(void)(argument);
	uart_handle_t test_uart1_handle;
	user_error_t sc;
	sc = uart_get_handle("uart1",sizeof("uart1"),&test_uart1_handle);
	if(sc == RET_OK){
		printf("test_uart1 get %s handle is %d\n","uart1",test_uart1_handle);
	}else{
		printf("test_uart1 get %s handle fail %d,exit test_uart1_task \n","uart1",sc);
		vTaskDelete(NULL);
	}
	uint16_t actual_size;
	while(1){
		sc = uart_write_data(test_uart1_handle, "this from uart1 test func hello\n", 
						sizeof("this from uart1 test func hello\n"),&actual_size);
		if(sc == RET_OK){
			printf("test_uart1 transmit ok\n");
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
		printf("test_uart1_function create fail\n");
	}else{
		printf("test_uart1_function create success\n");
	}
}


#endif

