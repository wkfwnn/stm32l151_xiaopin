#include "lock.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "infrared_detection_module.h"
#include "user_define.h"

#include "motor_lock.h"

extern motor_lock_task_struct_t motor_lock_task;

void lock_function(void const * argument)
{
	uint8_t status = 0x00;
	
	while(1){

		vTaskDelay(100);
		get_infrared_detection_status(&status);
		DBG_LOG("lock status is %x,current status is %d\n",status,motor_lock_task.motor_lock_status);
		
	}

}



void lock_task_create()
{
	osThreadId lock_thread_handle;
	osThreadDef(lock_task, lock_function, osPriorityLow, 0, 128);
	lock_thread_handle = osThreadCreate(osThread(lock_task),NULL);
	if(lock_thread_handle == NULL){
		DBG_LOG("lock_function create fail\n");
	}else{
		DBG_LOG("lock_function create success\n");
	}

}

