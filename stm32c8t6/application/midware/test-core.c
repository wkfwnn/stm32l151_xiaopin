#include "test-core.h"
#include "test_uart1.h"
#include "test_uart2.h"
#include "test_uart3.h"


void test_func_start(void)
{
#if TEST_UART1_FUNC
	test_uart1_func();
#endif
#if TEST_UART2_FUNC
	test_uart2_func();
#endif
#if TEST_UART3_FUNC
	test_uart3_func();
#endif

}

