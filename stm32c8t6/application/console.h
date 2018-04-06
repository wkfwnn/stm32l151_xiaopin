#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include "user_Define.h"
#include "stdint.h"

#define MAX_COMMOND_LEN   6
#define MAX_FUNCTION_INSTRUCTIONS_LEN (30)

typedef void (*call_back_function_p)(void);

typedef struct {
	char commond_name[MAX_COMMOND_LEN];
	uint8_t commond_name_len;
	call_back_function_p   func;
	char function_instructions[MAX_FUNCTION_INSTRUCTIONS_LEN];
}console_commond_map_t;

/*
notice:  commond_name end of '\0',max len do not exceed MAX_COMMOND_LEN including '\0',
		 commond_name_len is sizeof(*commond_name),func can  not be NULL,function_instructions
		 must end of '\0',sizeof(*function_instructions) can not exceed MAX_FUNCTION_INSTRUCTIONS_LEN-1
*/
user_error_t  console_commond_register(const char *commond_name,uint8_t commond_name_len,
									   call_back_function_p  func,
									   const char *function_instructions);
void console_task_create(void);


#endif 
