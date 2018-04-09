#ifndef __USER_DEFINE_H_
#define __USER_DEFINE_H_


typedef enum {
	RET_ERROR = -0x10,
	RET_BUSY,
	RET_TIME_OUT,
	RET_PARA_ERR,
	RET_PARA_NOT_COMPATIBLE,
	RET_UNKNOWN_PARAMETER,
	RET_OVER_FLOW,
	RET_DO_NOT_SUPPORT_CURRENT,
	RET_OK = 0x0
	
}user_error_t;


#ifndef NULL
	#define NULL (void*)0
#endif
	
	
typedef enum {
	FLAG_NO_BLOCK,
	FLAG_BLOCK,
	FLAG_WITH_TIMEOUT
}user_flag_t;

/*if you change DEBUG port, change this macro and find PUTCHAR_PROTOTYPE
	defined in console.c,change to other uart*/
#define UART_DEBUG_PORT    "uart2"

/*This is for test func*/
#define TEST_FUNC            1
#define TEST_UART1_FUNC      1
#define TEST_UART2_FUNC      1
#define TEST_UART3_FUNC      1



#endif
