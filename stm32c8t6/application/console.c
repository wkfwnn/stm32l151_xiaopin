#include "console.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "user_define.h"
#include "uart-core.h"
#include "string.h"

#define CONSOLE_COMMOND_MAX_LEN      10
#define CONSOLE_BUFF_COUNT          2

typedef struct {
	osThreadId task_handle;
	console_commond_map_t console_commond_map[CONSOLE_COMMOND_MAX_LEN];
	uint8_t commond_map_counter;
	uint8_t console_buff[CONSOLE_BUFF_COUNT][MAX_UART_QUEUE_SIZE];
	uint8_t console_buff_index;   //ping-pong buff
	uint8_t console_buff_size[CONSOLE_BUFF_COUNT];
	SemaphoreHandle_t xSemaphore;
}console_struct_t;


static console_struct_t console = {
	.commond_map_counter = 0x00,
	.console_buff_index = 0x00,
	.console_buff_size = {0x0,0x0},
	.xSemaphore = NULL
};

void help_function(void)
{
	
	
}

static void register_help_commond()
{
	user_error_t sc;
	
	sc = console_commond_register("help",sizeof("help"),help_function,"list all commond!");
	if(sc != RET_OK){
		printf("console_help_register fail %d\n",sc);
	}
}





void console_read_call_back(uint8_t *data,uint16_t size)
{
	#if 0
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	memcpy(console.console_buff[console.console_buff_index % CONSOLE_BUFF_COUNT],data,
		(console.console_buff_size[console.console_buff_index % CONSOLE_BUFF_COUNT] = (size >= MAX_UART_QUEUE_SIZE? MAX_UART_QUEUE_SIZE:size)));
	/* The event has occurred, use the semaphore to unblock the task so the task
	can process the event. */
	xSemaphoreGiveFromISR( console.xSemaphore, &xHigherPriorityTaskWoken );
	/* Clear the interrupt here. */
	/* Now the task has been unblocked a context switch should be performed if
	xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax required to perform
	a context switch from an ISR varies from port to port, and from compiler to
	compiler. Check the web documentation and examples for the port being used to
	find the syntax required for your application. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	#endif
	printf("console receive buff size is %d\n",size);	
}


void parsing_console_string(uint8_t *pdata,uint16_t size)
{
	uint8_t i;
	for(i = 0; i < console.commond_map_counter;i++){
			
	}

}

void console_task_function(void const * argument)
{
	uart_handle_t console_handle = 0;
	user_error_t sc;
	BaseType_t ret;
	sc = uart_get_handle("uart2",sizeof("uart1"),&console_handle);
	if(sc == RET_OK){
		printf("console get uart2 handle is %d\n",console_handle);
	}else{
		printf("console get uart2 handle fail %d,exit console_task \n",sc);
		vTaskDelete(console.task_handle);
		vTaskDelete(NULL);
	}
	sc = uart_core_read_register(console_handle,console_read_call_back);
	if(sc != RET_OK){
		printf("console core register fail %d\n",sc);
	}else{
		printf("console core register success \n");
	}
	console.xSemaphore = xSemaphoreCreateMutex();
	if( console.xSemaphore == NULL ){
		printf("console sem create fail\n");
	}
	while(1){
		 if(console.xSemaphore){
			ret = xSemaphoreTake(console.xSemaphore,portMAX_DELAY);
		 }else{
			vTaskSuspend(console.task_handle);
		 }
		 if(ret == pdPASS){
			parsing_console_string(console.console_buff[console.console_buff_index++ % CONSOLE_BUFF_COUNT],
								console.console_buff_size[console.console_buff_index++ % CONSOLE_BUFF_COUNT]);
		 }
	}
	
}

void console_task_create(void)
{
	
	osThreadDef(console_task, console_task_function, osPriorityNormal, 0, 128);
	console.task_handle = osThreadCreate(osThread(console_task), NULL);
	if(console.task_handle  == NULL){
		printf("console_task_function create fail\n");
	}else{
		printf("console_task_function create success\n");
	}
	register_help_commond();
}

/*
notice:
	commond_name  and instructions must end of '\0' 
*/
user_error_t  console_commond_register(const char *commond_name,uint8_t commond_name_len,
										   call_back_function_p  func,
										   const char *function_instructions)
{
	static uint8_t flag;
	user_error_t ret;
	if(flag == 1){
		ret =  RET_BUSY;
		goto return_status;
	}else{
		
		if(console.commond_map_counter > CONSOLE_COMMOND_MAX_LEN){
			ret =  RET_OVER_FLOW;
			goto return_status;
		}else{
			if(sizeof(*commond_name) != commond_name_len || 
				commond_name_len > (MAX_COMMOND_LEN-1) ||
				sizeof(*function_instructions) > (MAX_FUNCTION_INSTRUCTIONS_LEN -1)||
				func == NULL){
				ret = RET_PARA_ERR;
				goto return_status;
			}else{
				flag = 1;
				console.commond_map_counter++;
				memcpy(console.console_commond_map[console.commond_map_counter -1].commond_name,
					commond_name,sizeof(*commond_name));
				memcpy(console.console_commond_map[console.commond_map_counter -1].function_instructions,
					function_instructions,sizeof(*function_instructions));
				console.console_commond_map[console.commond_map_counter - 1].commond_name_len = commond_name_len;
				console.console_commond_map[console.commond_map_counter - 1].func = func;
				flag = 0;	
				ret =  RET_OK;
				goto return_status;
			}
		}	
		
	}
	
return_status:
	return ret;
}



