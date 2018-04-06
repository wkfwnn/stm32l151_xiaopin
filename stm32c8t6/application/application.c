#include "cmsis_os.h"
#include "task_state.h"
#include "application.h"
#include "console.h"
#include "uart-core.h"


void application_start()
{
	uart_core_read_task_create();
	create_state_task();
	console_task_create();
}

