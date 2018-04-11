#include "task_state.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "console.h"
#include "string.h"


static osThreadId task_state_handle;


void task_state_function(void const * argument)
{
	char buff[240] = {0x00,};
	uint8_t counter = 0;
	while(1){
		if(counter == 0){
			vTaskSuspend(NULL);
			counter++;
		}else{
		  if(counter++ < 4){
			vTaskList(buff);	
			printf("%s\n",buff);
			vTaskGetRunTimeStats(buff);
			printf("%s\n",buff);
			vTaskDelay(1000);
		  }else{
			counter = 0;
		  }
		}	
	}
}


void task_uart_commond_excute_func()
{
	vTaskResume(task_state_handle);
}

void create_state_task(void)
{
	user_error_t ret;
	osThreadDef(task_state, task_state_function, osPriorityLow, 0, 140);
	task_state_handle = osThreadCreate(osThread(task_state), NULL);
	if(task_state_handle == NULL){
		printf("task_state_function create fail\n");
	}else{
		printf("task_state_function create success\n");
	}
	ret = console_commond_register("CPU",strlen("CPU"),task_uart_commond_excute_func,"cpu usage,stack, left stack");
	if(ret != RET_OK){
		printf("cpu commond register fail\n");
	}
	
}






