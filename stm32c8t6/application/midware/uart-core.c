/*
	This file is about uart function,it will start a task about uart read
	only support receive  max bytes  defined in file:MAX_UART_QUEUE_SIZE,if
	exceeed MAX_UART_QUEUE_SIZE,we only receive MAX_UART_QUEUE_SIZE,mean bytes 
	exceed MAX_UART_QUEUE_SIZE will lost some ,so if want receive exceed MAX_UART_QUEUE_SIZE,
	please define more size than MAX_UART_QUEUE_SIZE
*/

#include "uart-core.h"
#include "user_define.h"
#include "stdio.h"
#include "stm32l1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"

/*this define in main.c*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;



typedef enum {
	UART_RX_OEPN,
	UART_RX_CLOSE,
}uart_core_rx_t;

#define UART_NAME_MAX_LEN  6
typedef struct {
	char uart_name[UART_NAME_MAX_LEN];
	uart_handle_t uart_handle;
	UART_HandleTypeDef *uart;
	uint8_t *huart_queue_p;
	/*call back function is in isr routing,pls as short as possible,fist copy data,and relese sem or other signal*/
	uart_read_call_back  uart_read_call_back_array[MAX_READ_CALL_BACK];
	uint8_t uart_call_back_count;
	uint8_t uart_rx_status;
	uint16_t read_event_bits;
	SemaphoreHandle_t xSemaphore;
	uint16_t write_event_bits;
}uart_map_t;


#define UART1_READ_CORE_BIT_0 ( 1 << 0 )
#define UART2_READ_CORE_BIT_1 ( 1 << 1 )
#define UART3_READ_CORE_BIT_2 ( 1 << 2 )
#define UART_CORE_READ_BIT_ALL (UART1_READ_CORE_BIT_0 | UART2_READ_CORE_BIT_1| UART3_READ_CORE_BIT_2)

#define UART1_WRITE_CORE_BIT_3 (1 << 3)
#define UART2_WRITE_CORE_BIT_4 (1 << 4)
#define UART3_WRITE_CORE_BIT_5 (1 << 5)
#define UART_CORE_WRITE_BIT_ALL (UART1_WRITE_CORE_BIT_3 | UART2_WRITE_CORE_BIT_4| UART3_WRITE_CORE_BIT_5)



#define MAX_UART_QUEUE_SIZE   100
uint8_t uart_queue1[MAX_UART_QUEUE_SIZE] = {0x0,};
uint8_t uart_queue2[MAX_UART_QUEUE_SIZE] = {0x0,};
uint8_t uart_queue3[MAX_UART_QUEUE_SIZE] = {0x0,};
uart_map_t   uart_map_array[] = {
	{"uart1",1,&huart1,uart_queue1,{NULL,NULL,NULL},0,UART_RX_CLOSE,UART1_READ_CORE_BIT_0,NULL,UART1_WRITE_CORE_BIT_3},
	{"uart2",2,&huart2,uart_queue2,{NULL,NULL,NULL},0,UART_RX_CLOSE,UART2_READ_CORE_BIT_1,NULL,UART2_WRITE_CORE_BIT_4},
	{"uart3",3,&huart3,uart_queue3,{NULL,NULL,NULL},0,UART_RX_CLOSE,UART3_READ_CORE_BIT_2,NULL,UART3_WRITE_CORE_BIT_5}
	//add other uart here
};



static EventGroupHandle_t uart_core_read_event_group;
static EventGroupHandle_t uart_core_write_event_group;

/*
@name:can be uart1,uart2,uart3,uart4...
@handle: return a handle,common uart_name return common handle 
*/
user_error_t uart_get_handle(const char *uart_name, uint8_t uart_name_len,uart_handle_t *handle)
{
	uint8_t i;
	if(uart_name_len >UART_NAME_MAX_LEN){
		return RET_PARA_ERR;
	}

	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(strncmp(uart_map_array[i].uart_name,uart_name,uart_name_len) == 0){
			*handle = uart_map_array[i].uart_handle;
			return   RET_OK;
		}
	}
	return RET_PARA_NOT_COMPATIBLE;
	
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	#if 1
	uint8_t i;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(uart_map_array[i].uart == huart){
			printf("%s receive or send fail\n",uart_map_array[i].uart_name);
			xResult = xEventGroupSetBitsFromISR(uart_core_read_event_group,uart_map_array[i].read_event_bits,
												&xHigherPriorityTaskWoken);
			/* Was the message posted successfully */
			if( xResult != pdFAIL ){
				/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
				switch should be requested. The macro used is port specific and will
				be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
				the documentation page for the port being used. */
				portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			}	
			xResult = xEventGroupSetBitsFromISR(uart_core_write_event_group,uart_map_array[i].write_event_bits,
												&xHigherPriorityTaskWoken);
			/* Was the message posted successfully */
			if( xResult != pdFAIL ){
				/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
				switch should be requested. The macro used is port specific and will
				be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
				the documentation page for the port being used. */
				portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			}	
			
		}
	}
	#endif
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback can be implemented in the user file
   */
}

/**
  * @brief  Rx Half Transfer completed callbacks.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxHalfCpltCallback can be implemented in the user file
   */
}

void HAL_UART_Rx_FrameCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t i,j = 0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(huart == uart_map_array[i].uart){
			for(j = 0;j < uart_map_array[i].uart_call_back_count;j++){
				if(uart_map_array[i].uart_read_call_back_array[j] != NULL){
					uart_map_array[i].uart_read_call_back_array[j](uart_map_array[i].huart_queue_p,huart->RxXferCount);
				}	
			}
			xResult = xEventGroupSetBitsFromISR(uart_core_read_event_group,uart_map_array[i].read_event_bits,
												&xHigherPriorityTaskWoken);
			/* Was the message posted successfully */
			if( xResult != pdFAIL ){
				/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
				switch should be requested. The macro used is port specific and will
				be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
				the documentation page for the port being used. */
				portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			}
		}

	}

}

/*
@pdata:data buff pointer
@data_size:need to write data size
@flag:
*/
HAL_StatusTypeDef  uart_read_one_frame_data(UART_HandleTypeDef *huart,uint8_t *pdata,uint16_t data_size)
{
	HAL_StatusTypeDef status;
	uint8_t count = 0;
reveive_dma:
	status = HAL_UART_Receive_Frame_DMA(huart,pdata,data_size);
	if(status != HAL_OK && status != HAL_BUSY){
		printf("HAL_UART_Receive_DMA fail  or busy %d\n",status);
	}else if(status == HAL_BUSY){
		vTaskDelay(1);
		if(count++ >100){
			return status;
		}
		goto reveive_dma;
	}
	return status;
}

user_error_t  uart_core_read_register(uart_handle_t handle,uart_read_call_back call_back_func)
{
	uint8_t i;
	user_error_t ret;
	EventBits_t uxBits;
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(uart_map_array[i].uart_handle == handle){	
			if(uart_map_array[i].uart_call_back_count++< MAX_READ_CALL_BACK){
				printf("i = %d,uart_call_back_count = %d\n",i,uart_map_array[i].uart_call_back_count);
				uart_map_array[i].uart_read_call_back_array[uart_map_array[i].uart_call_back_count-1] = call_back_func;
				//printf("uart -core register %s at %d\n",uart_map_array[i].uart_name,uart_map_array[i].uart_call_back_count);
				if(uart_map_array[i].uart_rx_status != UART_RX_OEPN){
					//printf("uart core start open uart read\n");
					uart_map_array[i].uart_rx_status = UART_RX_OEPN;
					uxBits = xEventGroupSetBits(uart_core_read_event_group,uart_map_array[i].read_event_bits);
					if( ( uxBits & ( UART_CORE_READ_BIT_ALL ) ) == ( UART_CORE_READ_BIT_ALL ) )
					{
					/*  bit 0 and bit 4 均被置位. */
					}
					else if( ( uxBits & UART1_READ_CORE_BIT_0 ) != 0 )
					{
					/* 只有Bit 0 被置位 */
					}
					else if( ( uxBits & UART2_READ_CORE_BIT_1 ) != 0 )
					{
					/* 只有Bit 4 被置位 . */
					}
					else if( ( uxBits & UART3_READ_CORE_BIT_2 ) != 0 )
					{
					/* 只有Bit 6 被置位 . */
					}
					else
					{
					/* 置位失败. */
					}
					printf("uxBits = %x\n",uxBits);
					ret = RET_OK;
					goto return_status;
				}else{
					ret = RET_ERROR;
					goto return_status;
				}
			}else{
				ret = RET_OVER_FLOW;
				goto return_status;
			}
		}
	}
	ret = RET_ERROR;
	
return_status:
	return ret;

}

void uart_core_read_task_function(void const * argument)
{
	UNUSED(argument);
	
	while(1){
		EventBits_t uxBits;
		const TickType_t xTicksToWait = pdMS_TO_TICKS( portMAX_DELAY);
		uxBits = xEventGroupWaitBits( uart_core_read_event_group,UART_CORE_READ_BIT_ALL,
							          pdTRUE,pdFALSE,xTicksToWait);
		//	
		//uart_read_one_frame_data(&huart2,uart_map_array[1].huart_queue_p,MAX_UART_QUEUE_SIZE);
		//uart_read_one_frame_data(&huart3,uart_map_array[2].huart_queue_p,MAX_UART_QUEUE_SIZE);
		if( ( uxBits & ( UART_CORE_READ_BIT_ALL ) == ( UART_CORE_READ_BIT_ALL ))){
		
		}else if( ( uxBits & UART1_READ_CORE_BIT_0 ) != 0 ){
			uart_read_one_frame_data(&huart1,uart_map_array[0].huart_queue_p,MAX_UART_QUEUE_SIZE);
			/* xEventGroupWaitBits() returned because just BIT_0 was set. */
		}else if( ( uxBits & UART2_READ_CORE_BIT_1 ) != 0 ){
			uart_read_one_frame_data(&huart2,uart_map_array[1].huart_queue_p,MAX_UART_QUEUE_SIZE);
			//printf("uart2 start readhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
		/* xEventGroupWaitBits() returned because just BIT_4 was set. */
		}else if( ( uxBits & UART3_READ_CORE_BIT_2 ) != 0 ){
			uart_read_one_frame_data(&huart3,uart_map_array[2].huart_queue_p,MAX_UART_QUEUE_SIZE);
			//printf("uart3 start read\n");
			/* xEventGroupWaitBits() returned because just BIT_6 was set. */
		}else{
		/* xEventGroupWaitBits() returned because xTicksToWait ticks passed
		without either BIT_0 or BIT_4 becoming set. */
		}
		//printf("nidaye\n");

	}
}


/********************************************************************************
	This is uart-core write funciton,including tx call back,tx funcition
********************************************************************************/
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
#if 0
	uint8_t i;
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(uart_map_array[i].uart == huart){
			if(uart_map_array[i].task_id != NULL){
				vTaskResume(uart_map_array[i].task_id);
			}
		}
	}
#endif
	
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	
	uint8_t i;
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(uart_map_array[i].uart == huart){
			xResult = xEventGroupSetBitsFromISR(uart_core_write_event_group,uart_map_array[i].write_event_bits,
												&xHigherPriorityTaskWoken);
			/* Was the message posted successfully */
			if( xResult != pdFAIL ){
				/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
				switch should be requested. The macro used is port specific and will
				be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
				the documentation page for the port being used. */
				portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			}	
		}
	}
}

/*
@pdata:data buff pointer
@data_size:need to write data size
@actual_size:return actual_size transmit
@notice:in this funciton is block transfer
*/
user_error_t uart_write_data(uart_handle_t handle,uint8_t *pdata,uint16_t data_size)
{
	user_error_t ret = RET_OK;
	
	HAL_StatusTypeDef hal_status;
	UART_HandleTypeDef *huart = NULL;
	uint8_t count = 0;
	EventBits_t uxBits;
	uint8_t i = 0;
	//jutice para of data size and data pointer
	if(data_size == 0 || pdata == NULL){
		ret = RET_PARA_ERR;
		goto return_status;
	}
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(uart_map_array[i].uart_handle == handle){
			if(uart_map_array[i].xSemaphore != NULL){
				huart = uart_map_array[i].uart;
				xSemaphoreTake( uart_map_array[i].xSemaphore,portMAX_DELAY);
				goto dma_transmit;
				 
			}			
		}
	}
	if(huart == NULL){
		ret = RET_DO_NOT_SUPPORT_CURRENT;
		goto return_status;
	}
	
dma_transmit:
	hal_status  = HAL_UART_Transmit_DMA(huart,pdata,data_size);
	if(hal_status == HAL_OK){
		uxBits = xEventGroupWaitBits( uart_core_write_event_group,uart_map_array[i].write_event_bits,
							          pdTRUE,pdFALSE,portMAX_DELAY);
		if((uxBits & uart_map_array[i].write_event_bits) != 0){
			if((uart_map_array[i].uart->ErrorCode & HAL_UART_ERROR_DMA) != 0){
				uart_map_array[i].uart->ErrorCode&= (~HAL_UART_ERROR_DMA);
				ret = RET_ERROR;
			}else{
				ret = RET_OK;	
			}
			xSemaphoreGive(uart_map_array[i].xSemaphore);
			goto return_status;
			
		}
	}else if(hal_status == HAL_BUSY){
		vTaskDelay(1);
		if(count++ > 100){
			printf("uart core %s write data fail\n",uart_map_array[i].uart_name);
			xSemaphoreGive(uart_map_array[i].xSemaphore);
			ret = RET_TIME_OUT;
			goto return_status;
		}
		goto dma_transmit;	
	}
	
return_status:
	return ret;
}


void uart_core_task_create()
{
	/*uart read task */
	osThreadId uart_core_task_read_handle;
	osThreadDef(uart_read_core, uart_core_read_task_function, osPriorityNormal, 0, 64);
	uart_core_task_read_handle = osThreadCreate(osThread(uart_read_core), NULL);
	
	if(uart_core_task_read_handle == NULL){
		printf("uart_core_read_task_function create fail\n");
	}else{
		printf("uart_core_read_task_function create success\n");
	}
	/* Attempt to create the event group. */
	uart_core_read_event_group = xEventGroupCreate();
	/* Was the event group created successfully */
	if( uart_core_read_event_group == NULL ){
		printf("uart core read group create fail\n");	
	}
	else{		
		printf("uart core read group create success\n");
		xEventGroupClearBits(uart_core_read_event_group,(UART_CORE_READ_BIT_ALL));
	}
	
	uint8_t i;
	for(i = 0;i <sizeof(uart_map_array)/sizeof(uart_map_t);i++){
		if(strncmp(uart_map_array[i].uart_name,UART_DEBUG_PORT,strlen(uart_map_array[i].uart_name))!= 0){
			vSemaphoreCreateBinary(uart_map_array[i].xSemaphore);
			if(uart_map_array[i].xSemaphore == NULL){
				printf("%s seam create fail\n",uart_map_array[i].uart_name);
			}
		}
	}

	/* Attempt to create the event group. */
	uart_core_write_event_group = xEventGroupCreate();
	/* Was the event group created successfully */
	if( uart_core_write_event_group == NULL ){
		printf("uart core write group create fail\n");	
	}
	else{		
		printf("uart core write group create success\n");
		xEventGroupClearBits(uart_core_write_event_group,(UART_CORE_WRITE_BIT_ALL));
	}
	
}
