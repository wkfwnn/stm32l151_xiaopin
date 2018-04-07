#include "cmsis_os.h"
#include "task_state.h"
#include "application.h"
#include "console.h"
#include "uart-core.h"
#include "test-core.h"


void application_start()
{
	uart_core_task_create();
	create_state_task();
	console_task_create();
#if TEST_FUNC
	test_func_start();
#endif
}
