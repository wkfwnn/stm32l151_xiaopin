#include "ble_bluetooth.h"
#include "ble_bluetooth_module.h"
#include "string.h"

#define BLE_TASK_EVENT_BITS         (1 <<0)

typedef struct {
	osThreadId task_handle;
	uint8_t ble_task_buff[MAX_UART_QUEUE_SIZE];
	uint8_t ble_task_size;
	EventGroupHandle_t ble_task_event_handle;
}ble_task_struct_t;


ble_task_struct_t ble_task = {
	.task_handle = 0x00,
	.ble_task_size = 0x00,
	.ble_task_event_handle = NULL
};


void ble_task_data_call_back(uint8_t *data,uint16_t size)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	memcpy(ble_task.ble_task_buff,data,
		  (ble_task.ble_task_size = (size >= MAX_UART_QUEUE_SIZE? MAX_UART_QUEUE_SIZE:size)));
	#if 1
	xResult = xEventGroupSetBitsFromISR(ble_task.ble_task_event_handle,BLE_TASK_EVENT_BITS,
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
	xResult = xTaskResumeFromISR(ble_task.task_handle);
	if(xResult != pdTRUE ){
		printf("ble task resume err\n");		

	}
	#endif	
}

void ble_task_function(void const * argument)
{
	int sc = 0x00,i = 0;
	sc =  ble_data_read_call_back_register(ble_task_data_call_back);
	if(sc != RET_OK){
		printf("ble task register ble data read fail %d\n",sc);
	}
	
	while(1){
		EventBits_t uxBits;
		const TickType_t xTicksToWait = pdMS_TO_TICKS( portMAX_DELAY);
		 if(ble_task.ble_task_event_handle){
			uxBits = xEventGroupWaitBits( ble_task.ble_task_event_handle,BLE_TASK_EVENT_BITS,
						          pdTRUE,pdFALSE,xTicksToWait);	
			if((uxBits & BLE_TASK_EVENT_BITS) != 0 ){
				for(i = 0;i < ble_task.ble_task_size;i++)
					printf("%c",ble_task.ble_task_buff[i]);
				printf("\n");
			}






		 }
	

	}

}


void ble_task_create(void)
{
	osThreadDef(ble_task, ble_task_function, osPriorityNormal, 0, 128);
	ble_task.task_handle = osThreadCreate(osThread(ble_task), NULL);
	if(ble_task.task_handle  == NULL){
		printf("ble_task_function create fail\n");
	}else{
		printf("ble_task_function create success\n");
	}
	ble_task.ble_task_event_handle = xEventGroupCreate();
	/* Was the event group created successfully */
	if( ble_task.ble_task_event_handle == NULL ){
		printf("ble_task.ble_task_event_handle group create fail\n");	
	}
	else{		
		printf("ble_task.ble_task_event_handle create success\n");
		xEventGroupClearBits(ble_task.ble_task_event_handle,(BLE_TASK_EVENT_BITS));
	}
	
	
}




