#include "console.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "user_define.h"
#include "uart-core.h"
#include "string.h"
#include "stm32l1xx_hal.h"


#define CONSOLE_COMMOND_MAX_LEN      10
#define CONSOLE_BUFF_COUNT          2
#define CONSOLE_EVENT_BITS         (1 <<0)

typedef struct {
	osThreadId task_handle;
	console_commond_map_t console_commond_map[CONSOLE_COMMOND_MAX_LEN];
	uint8_t commond_map_counter;
	uint8_t console_buff[MAX_UART_QUEUE_SIZE];
	uint8_t console_buff_size;
	EventGroupHandle_t console_event_handle;
}console_struct_t;


static console_struct_t console = {
	.commond_map_counter = 0x00,
	.console_buff_size =0x0,
	.console_event_handle = NULL
};

void help_function(void)
{
	uint8_t i;
	DBG_LOG("commond:     instructions:\n");
	for(i = 0; i < console.commond_map_counter;i++){
		DBG_LOG("%s          %s\n", console.console_commond_map[i].commond_name,
								 console.console_commond_map[i].function_instructions); 
	}
	
	
}

static void register_help_commond()
{
	user_error_t sc;
	
	sc = console_commond_register("help",strlen("help"),help_function,"list all commond!");
	if(sc != RET_OK){
		DBG_LOG("console_help_register fail %d\n",sc);
	}
}





void console_read_call_back(uint8_t *data,uint16_t size)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	memcpy(console.console_buff,data,
		  (console.console_buff_size = (size >= MAX_UART_QUEUE_SIZE? MAX_UART_QUEUE_SIZE:size)));
	#if 1
	xResult = xEventGroupSetBitsFromISR(console.console_event_handle,CONSOLE_EVENT_BITS,
												&xHigherPriorityTaskWoken);
	/* Was the message posted successfully */
	if( xResult != pdFAIL ){
		/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
		switch should be requested. The macro used is port specific and will
		be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
		the documentation page for the port being used. */
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	#else
	xResult = xTaskResumeFromISR(console.task_handle);
	if(xResult != pdTRUE ){
		DBG_LOG_ISR("console task resume err\n");		

	}
	#endif
	
	//DBG_LOG_ISR("console receive buff size is %d\n",size);	
}


void parsing_console_string(uint8_t *pdata,uint16_t size)
{
	uint8_t i;
	uint8_t commond_found_flag = 0;
//	for(i = 0;i < size;i++){
//		DBG_LOG("%c ",pdata[i]);
//	}
//	DBG_LOG("\n");
//	DBG_LOG("map count = %d\n",console.commond_map_counter);
	for(i = 0; i < console.commond_map_counter;i++){
		//DBG_LOG("name is %s.len is %d\n",console.console_commond_map[i].commond_name,console.console_commond_map[i].commond_name_len);
		if(strncmp(console.console_commond_map[i].commond_name,(const char *)pdata,console.console_commond_map[i].commond_name_len) == 0){
			commond_found_flag = 1;
			console.console_commond_map[i].func();	
		}
	}
	if(commond_found_flag != 1){
		DBG_LOG("commond can not found\n");
	}
}

void console_task_function(void const * argument)
{
	uart_handle_t console_handle = 0;
	user_error_t sc;
	sc = uart_get_handle(UART_DEBUG_PORT,sizeof(UART_DEBUG_PORT),&console_handle);
	if(sc == RET_OK){
		DBG_LOG("console get %s handle is %d\n",UART_DEBUG_PORT,console_handle);
	}else{
		DBG_LOG("console get %s handle fail %d,exit console_task \n",UART_DEBUG_PORT,sc);
		vTaskDelete(console.task_handle);
		vTaskDelete(NULL);
	}
	sc = uart_core_read_register(console_handle,console_read_call_back);
	if(sc != RET_OK){
		DBG_LOG("console core register fail %d\n",sc);
	}else{
		DBG_LOG("console core register success \n");
	}
	while(1){
		#if 1
		EventBits_t uxBits;
		const TickType_t xTicksToWait = pdMS_TO_TICKS( portMAX_DELAY);
		 if(console.console_event_handle){
			uxBits = xEventGroupWaitBits( console.console_event_handle,CONSOLE_EVENT_BITS,
						          pdTRUE,pdFALSE,xTicksToWait);
			if((uxBits & CONSOLE_EVENT_BITS) != 0){
				parsing_console_string(console.console_buff,console.console_buff_size);
				memset(console.console_buff,0x00,console.console_buff_size);	
			}else{
				DBG_LOG("console receive unaccepted event\n");
			}
		 }else{
			vTaskSuspend(console.task_handle);
		 }
		 #else
			 vTaskSuspend(console.task_handle);
			 parsing_console_string(console.console_buff,console.console_buff_size);
			 memset(console.console_buff,0x00,console.console_buff_size);	
		 #endif
	}
	
}

void console_task_create(void)
{
	osThreadDef(console_task, console_task_function, osPriorityNormal, 0, 128);
	console.task_handle = osThreadCreate(osThread(console_task), NULL);
	if(console.task_handle  == NULL){
		DBG_LOG("console_task_function create fail\n");
	}else{
		DBG_LOG("console_task_function create success\n");
	}
	console.console_event_handle = xEventGroupCreate();
	/* Was the event group created successfully */
	if( console.console_event_handle == NULL ){
		DBG_LOG("console.console_event_handle group create fail\n");	
	}
	else{		
		DBG_LOG("console.console_event_handle create success\n");
		xEventGroupClearBits(console.console_event_handle,(CONSOLE_EVENT_BITS));
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
	//DBG_LOG("commond name :%s, commond len is %d,acual len is %d",commond_name,commond_name_len,strlen(commond_name));
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
			if(strlen(commond_name) != commond_name_len || 
				commond_name_len > (MAX_COMMOND_LEN-1) ||
				strlen(function_instructions) > (MAX_FUNCTION_INSTRUCTIONS_LEN -1)||
				func == NULL){
				ret = RET_PARA_ERR;
				goto return_status;
			}else{
				flag = 1;
				console.commond_map_counter++;
				memcpy(console.console_commond_map[console.commond_map_counter -1].commond_name,
					commond_name,strlen(commond_name));
				memcpy(console.console_commond_map[console.commond_map_counter -1].function_instructions,
					function_instructions,strlen(function_instructions));
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

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/** 
* @brief  Retargets the C library printf function to the USART. 
* @param  None 
* @retval None 
*/  
PUTCHAR_PROTOTYPE  
{  
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch,1,0xFF);
	return ch;
} 




