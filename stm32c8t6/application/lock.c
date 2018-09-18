#include "lock.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "infrared_detection_module.h"


void lock_function(void const * argument)
{
	uint8_t status = 0x00;
	while(1){

		vTaskDelay(10);
		//get_infrared_detection_status(&status);
		//printf("lock status is %x\n",status);
		
	}




}



void lock_task_create()
{
	osThreadId lock_thread_handle;
	osThreadDef(lock_task, lock_function, osPriorityLow, 0, 128);
	lock_thread_handle = osThreadCreate(osThread(lock_task),NULL);
	if(lock_thread_handle == NULL){
		printf("lock_function create fail\n");
	}else{
		printf("lock_function create success\n");
	}

}

