#include "ble_bluetooth_module.h"
#include "user_define.h"


/*引脚  I/O  功能及使用说明
GND  --
外部控制器需要和模块的 GND 共地
VCC  --
供电电压为 DC:2.0V—3.6V（推荐使用 3.3V）
P0_0  RTS
使能模块串口功能，默认高电平（模块
与外部 MCU 通信时，引脚必须拉低）
L：模块串口使能
H：模块串口关闭
P0_1  CMD
透传/命令控制，高电平表示透传数据，低电平表示命令；
默认不外接，该引脚是高电平。
P0_1 为高电平,所有的串口数据将被认为是透传数据,
P0_1 为低电平,所有的串口数据将被认为是命令
P0_2  RX
模块串口接收端 UART0_RX，连接外部控制器串口的 TX 端
P0_3  TX
模块串口发送端 UART0_TX，连接外部控制器串口的 RX 端
P0_4  SET
恢复出厂设置，模块上电时被拉低 2S 以上，即可恢复默认设置
P0_6  CTS
模块串口向外部 MCU 发送数据时，此引脚在串口发送数据前
800us 会拉低电平以通知外部 MCU，当数据发送完毕，此引脚
变为高电平。（如果外部 MCU 的串口一直打开，无需此类通知
信号，可以忽略此引脚，完全不影响应用）。
P1_0  CONN
模块网络状态指示，输出高电平则表示蓝牙已经连接
P2_0  SLEEP
睡眠控制引脚 (模块上电时广
播，P2_0 默认为高阻态)
L:模块唤醒，正常工作模式；
H:模块进入深度睡眠状态，停
止广播，断开蓝牙连接，此时
睡眠电流在 1.5uA 左右。
*/


#define enable_serial_port()  (HAL_GPIO_WritePin(BLE_GPIO_GROUP,BLE_MODULE_RTS_P0_0 ,GPIO_PIN_RESET))
#define disable_serial_port()  (HAL_GPIO_WritePin(BLE_GPIO_GROUP,BLE_MODULE_RTS_P0_0 ,GPIO_PIN_SET))
#define switch_to_cmd_mode()   (HAL_GPIO_WritePin(BLE_GPIO_GROUP,BLE_MODULE_CMD_P0_1 ,GPIO_PIN_RESET))
#define switch_to_data_mode()   (HAL_GPIO_WritePin(BLE_GPIO_GROUP,BLE_MODULE_CMD_P0_1 ,GPIO_PIN_SET))
#define exit_sleep_mode()      (HAL_GPIO_WritePin(BLE_GPIO_GROUP,BLE_MODULE_SLEEP_P2_0 ,GPIO_PIN_RESET))
#define enter_sleep_mode()    (HAL_GPIO_WritePin(BLE_GPIO_GROUP,BLE_MODULE_SLEEP_P2_0 ,GPIO_PIN_SET))



ble_module_private_data_t  ble_module = {
	.handle = 0x00,
	.ble_call_back_count = 0x00,
	.ble_call_back_map = {NULL,}
};

void module_self_test()
{	
	exit_sleep_mode();
	enable_serial_port();			
	switch_to_data_mode();

}

void ble_uart_read_call_back(uint8_t *data,uint16_t size)
{
	uint8_t i = 0;
	for (i = 0; i < ble_module.ble_call_back_count; ++i){
		ble_module.ble_call_back_map[i](data,size);
	}
}


void ble_module_start()
{
	int sc;
	//self test
	module_self_test();	
	
	sc = uart_get_handle(BLE_UART,sizeof(BLE_UART),&ble_module.handle);
	if(sc == RET_OK){
		DBG_LOG("ble get %s handle is %d\n",BLE_UART,ble_module.handle);
	}else{
		DBG_LOG("ble  get %s handle fail %d,exit console_task \n",BLE_UART,sc);
	}
	sc = uart_core_read_register(ble_module.handle,ble_uart_read_call_back);
	if(sc != RET_OK){
		DBG_LOG("ble uart register fail %d\n",sc);
	}else{
		DBG_LOG("ble uart register success \n");
	}	
}


int ble_data_read_call_back_register(ble_read_call_back call_back)
{
	if(call_back == NULL){
		return RET_ERROR;
	}
	if(ble_module.ble_call_back_count == BLE_MAX_CALL_BACK_COUNT){
		return RET_ERROR;
	}
	ble_module.ble_call_back_count++;
	ble_module.ble_call_back_map[ble_module.ble_call_back_count -1 ] = call_back;
	return RET_OK;
}


int ble_module_send_data(uint8_t *data,uint16_t data_len)
{
	int sc = 0x00;
	
	sc = uart_write_data(ble_module.handle,data,data_len);
	
	return sc;
}



