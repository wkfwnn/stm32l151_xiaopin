#include "motor_lock.h"
#include "ble_bluetooth_module.h"
#include "string.h"
#include "motor_module.h"
#include "speak_module.h"
#include "infrared_detection_module.h"
#include "log_module.h"


#define MOTOR_LOCK_CONTROL_MODE_BY_HOST_CMD            0x00
#define MOTOR_LOCK_CONTROL_MODE_BY_ILLEGAL_OPERATE     0x01

#define MOTOR_LOCK_TASK_BLE_DATA_EVENT_BITS         (1 << 0)
#define MOTOR_LOCK_TASK_INFRARED_DETECTION_STATUS_EVENT_BITS         (1 << 1)




motor_lock_task_struct_t motor_lock_task = {
	.task_handle = 0x00,
	.motor_lock_task_size = 0x00,
	.motor_lock_task_event_handle = NULL,
	.motor_lock_status = 0x00,
	.motor_lock_control_mode = MOTOR_LOCK_CONTROL_MODE_BY_ILLEGAL_OPERATE
};


typedef enum {
	MOTOR_LOCK_OPEN_LOCK = 0x0,
	MOTOR_LOCK_CLOSE_LOCK
}motor_lock_cmd_t;



void motor_lock_task_data_call_back(uint8_t *data,uint16_t size)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	memcpy(motor_lock_task.motor_lock_task_buff,data,
		  (motor_lock_task.motor_lock_task_size = (size >= MAX_UART_QUEUE_SIZE? MAX_UART_QUEUE_SIZE:size)));
	#if 1
	xResult = xEventGroupSetBitsFromISR(motor_lock_task.motor_lock_task_event_handle,MOTOR_LOCK_TASK_BLE_DATA_EVENT_BITS,
												&xHigherPriorityTaskWoken);
	/* Was the message posted successfully */
	if( xResult != pdFAIL ){
		/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
		switch should be requested. The macro used is port specific and will
		be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
		the documentation page for the port being used. */
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken);
	}
	#else
	xResult = xTaskResumeFromISR(motor_lock_task.task_handle);
	if(xResult != pdTRUE ){
		DBG_LOG_ISR("motor_lock task resume err\n");		

	}
	#endif	
}

void infrared_detection_callback()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE,xResult;
	if(motor_lock_task.motor_lock_control_mode == MOTOR_LOCK_CONTROL_MODE_BY_HOST_CMD){
		//do nothing
		DBG_LOG_ISR("infrared_detection_callback@1\n");
	}else if(motor_lock_task.motor_lock_control_mode == MOTOR_LOCK_CONTROL_MODE_BY_ILLEGAL_OPERATE){
		DBG_LOG_ISR("infrared_detection_callback@2\n");
		xResult = xEventGroupSetBitsFromISR(motor_lock_task.motor_lock_task_event_handle,MOTOR_LOCK_TASK_INFRARED_DETECTION_STATUS_EVENT_BITS,
													&xHigherPriorityTaskWoken);
		/* Was the message posted successfully */
		if( xResult != pdFAIL ){
			/* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
			switch should be requested. The macro used is port specific and will
			be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
			the documentation page for the port being used. */
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken);
		}	
	}else {
	}	
}

uint8_t open_lock()
{
	uint8_t status = 0x00;
	uint8_t ret = RET_OK;

	DBG_LOG("open lock\n");
	
	get_infrared_detection_status(&status);

	if(status == MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE){
		//dothing 	
		ret = RET_OK;
		goto return_value;
	}else{
		DBG_LOG("set status %d to %d\n",status,MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE);
		
		motor_lock_task.motor_lock_control_mode = MOTOR_LOCK_CONTROL_MODE_BY_HOST_CMD;
		set_speak_out_put(SPEAK_STATUS_ALARM);
		ret = set_motor_module_status(status,MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE);
		set_speak_out_put(SPEAK_STATUS_MUTE);	
		motor_lock_task.motor_lock_control_mode = MOTOR_LOCK_CONTROL_MODE_BY_ILLEGAL_OPERATE;
		
		if(ret == RET_OK){
			motor_lock_task.motor_lock_status = MOTOR_MODULE_STATUS_UPRIGHT_90_DEGREE;
			
		}else{
			//Return to the original state
			get_infrared_detection_status(&status);
			
			set_motor_module_status(status,motor_lock_task.motor_lock_status);
		}
		DBG_LOG("open lock return %d\n",ret);
	}
	
return_value:
	return ret;

}

uint8_t  close_lock()
{
	uint8_t status = 0x00;
	uint8_t ret = RET_OK;
	
	DBG_LOG("close lock\n");	
	
	get_infrared_detection_status(&status);

	if(status == MOTOR_MODULE_STATUS_FLAT_0_DEGREE){
		//dothing 	
		ret = RET_OK;
		goto return_value;
	}else{
		DBG_LOG("set status %d to %d\n",status,MOTOR_MODULE_STATUS_FLAT_0_DEGREE);
		
		motor_lock_task.motor_lock_control_mode = MOTOR_LOCK_CONTROL_MODE_BY_HOST_CMD;
		
		set_speak_out_put(SPEAK_STATUS_ALARM);
		
		ret = set_motor_module_status(status,MOTOR_MODULE_STATUS_FLAT_0_DEGREE);
		
		set_speak_out_put(SPEAK_STATUS_MUTE);	
		
		motor_lock_task.motor_lock_control_mode = MOTOR_LOCK_CONTROL_MODE_BY_ILLEGAL_OPERATE;
		
		if(ret == RET_OK){
			
			motor_lock_task.motor_lock_status = MOTOR_MODULE_STATUS_FLAT_0_DEGREE;
			
		}else{
			//Return to the original state
			get_infrared_detection_status(&status);
			
			set_motor_module_status(status,motor_lock_task.motor_lock_status);
		}
		
		DBG_LOG("close lock return %d\n",ret);
	}
	
return_value:
	return ret;


}

void infrared_detection_deal()
{
	uint8_t status = 0x00;
	uint8_t ret = RET_OK;

	DBG_LOG("infrared_detection_deal\n");
	
	get_infrared_detection_status(&status);	
	if(status == motor_lock_task.motor_lock_status){
		DBG_LOG("do not need set status %d\n",status);
	}else{
	
		set_speak_out_put(SPEAK_STATUS_ALARM);

		DBG_LOG("set status from %d to %d\n",status,motor_lock_task.motor_lock_status);
		
		ret = set_motor_module_status(status,motor_lock_task.motor_lock_status);
		
		set_speak_out_put(SPEAK_STATUS_MUTE);
		
		if(ret != RET_OK){
			
			get_infrared_detection_status(&status);	
			
			DBG_LOG("set status %d fail,",motor_lock_task.motor_lock_status);
		
			motor_lock_task.motor_lock_status = status;
			
			DBG_LOG("so set current status is %d\n",motor_lock_task.motor_lock_status);
		}else {
			
		}

	}

	


}


void motor_lock_data_deal(uint8_t size,uint8_t *data)
{
	uint8_t i = 0;
	
	//for(i = 0;i < motor_lock_task.motor_lock_task_size;i++)
	//	DBG_LOG("%x",motor_lock_task.motor_lock_task_buff[i]);
	//DBG_LOG("\n"); 
	
	uint8_t data_ret[2] = {0x00,};
	uint8_t ret = RET_OK;
	if(size != 1){
		ret = RET_ERROR;
	}else{
		switch (data[0]){
			case MOTOR_LOCK_OPEN_LOCK:
				ret = open_lock();
				break;
			case MOTOR_LOCK_CLOSE_LOCK:
				ret = close_lock();
				break;
			default:
				ret = RET_ERROR;
				
		}
		
	}
	data_ret[0] = data[0];
	data_ret[1] = ret;
	ret = ble_module_send_data(data_ret,sizeof(data_ret));
	if(ret != RET_OK){
		DBG_LOG("motor_lock send data fail\n");
	}
}


void motor_lock_task_function(void const * argument)
{
	
	
	while(1){
		
		EventBits_t uxBits;
		const TickType_t xTicksToWait = pdMS_TO_TICKS( portMAX_DELAY);
		 if(motor_lock_task.motor_lock_task_event_handle){
			uxBits = xEventGroupWaitBits( motor_lock_task.motor_lock_task_event_handle,MOTOR_LOCK_TASK_BLE_DATA_EVENT_BITS | MOTOR_LOCK_TASK_INFRARED_DETECTION_STATUS_EVENT_BITS,
						          pdTRUE,pdFALSE,xTicksToWait);	
			if((uxBits & MOTOR_LOCK_TASK_BLE_DATA_EVENT_BITS) != 0 ){
				motor_lock_data_deal(motor_lock_task.motor_lock_task_size,motor_lock_task.motor_lock_task_buff);
			}
			if((uxBits & MOTOR_LOCK_TASK_INFRARED_DETECTION_STATUS_EVENT_BITS) != 0 ){
				infrared_detection_deal();
			}
		 }
	

	}

}


void motor_lock_task_create(void)
{
	int sc = 0x00;
	osThreadDef(motor_lock_task_thread, motor_lock_task_function, osPriorityNormal, 0, 128);
	motor_lock_task.task_handle = osThreadCreate(osThread(motor_lock_task_thread), NULL);
	if(motor_lock_task.task_handle  == NULL){
		DBG_LOG("motor_lock_task_function create fail\n");
	}else{
		DBG_LOG("motor_lock_task_function create success\n");
	}
	motor_lock_task.motor_lock_task_event_handle = xEventGroupCreate();
	/* Was the event group created successfully */
	if( motor_lock_task.motor_lock_task_event_handle == NULL ){
		DBG_LOG("motor_lock_task.motor_lock_task_event_handle group create fail\n");	
	}
	else{		
		DBG_LOG("motor_lock_task.motor_lock_task_event_handle create success\n");
		xEventGroupClearBits(motor_lock_task.motor_lock_task_event_handle,(MOTOR_LOCK_TASK_BLE_DATA_EVENT_BITS));
		xEventGroupClearBits(motor_lock_task.motor_lock_task_event_handle,(MOTOR_LOCK_TASK_INFRARED_DETECTION_STATUS_EVENT_BITS));
	}
	
	sc =  ble_data_read_call_back_register(motor_lock_task_data_call_back);
	if(sc != RET_OK){
		DBG_LOG("motor_lock task register motor_lock data read fail %d\n",sc);
	}
	sc = register_infrared_detection_status_call_back(infrared_detection_callback);
	if(sc != RET_OK){
		DBG_LOG("motor_lock task register infrared_detection_status call back fail %d\n",sc);
	}

	get_infrared_detection_status(&motor_lock_task.motor_lock_status);
	
	DBG_LOG("current status is %d\n",motor_lock_task.motor_lock_status);
	
	
}

