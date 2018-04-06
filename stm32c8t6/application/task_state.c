#include "task_state.h"
#include "cmsis_os.h"
#include "stdio.h"


//static osThreadId task_state_handle;


void task_state_function(void const * argument)
{
	char buff[152] = {0x00,};
	while(1){
		vTaskList(buff);	
		printf("%s\n",buff);
		
		vTaskGetRunTimeStats(buff);
		printf("%s\n",buff);
			
		vTaskDelay( 5000);
		
	}




}


void create_state_task(void)
{
	osThreadId task_state_handle;
	osThreadDef(task_state, task_state_function, osPriorityLow, 0, 128);
	task_state_handle = osThreadCreate(osThread(task_state), NULL);
	if(task_state_handle == NULL){
		printf("task_state_function create fail\n");
	}else{
		printf("task_state_function create success\n");
	}
	
}






