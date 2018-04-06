#ifndef __UART_CORE_H_
#define __UART_CORE_H_

#include "user_define.h"
#include "stdint.h"
#include "cmsis_os.h"

typedef uint8_t uart_handle_t;
typedef void (*uart_read_call_back)(uint8_t *data,uint16_t size);

#define MAX_READ_CALL_BACK           3
#define MAX_UART_QUEUE_SIZE         100

typedef struct {
	user_flag_t flag;
	uint32_t time_out;
	TaskHandle_t task_id;
}uart_mode_t;

void uart_core_read_task_create(void);
user_error_t uart_get_handle(const char *uart_name, uint8_t uart_name_len,uart_handle_t *handle);
user_error_t  uart_core_read_register(uart_handle_t handle,uart_read_call_back call_back_func);


#endif
