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

#endif
