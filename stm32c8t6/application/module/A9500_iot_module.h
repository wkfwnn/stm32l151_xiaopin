#ifndef _A9500_IOT_H_
#define _A9500_IOT_H_
/**************************************
			AT COMMOND
note:
$:mean use ?

**************************************/

//module normal open 
//Manufacturer data
#define ATI       "ATI\r"
//set result is data mode
#define ATV0       "ATV0\r"
//inut pin code,look up pin 
#define AT_CPIN$    "AT+CPIN?\n"
//look up signal quality
#define AT_CSQ    "AT+CSQ"
//net register signal auto report 
#define AT_CEREG1  "AT+CEREG=1\r"
// look up carrieroperator information
#define AT_COPS$   "AT+COPS?\r"

//module TCP/IP data transfer
//set china mobile APN :CMNET 
#define AT_LSIPPROFILE1_CMNET "AT+LSIPPROFILE=1,\"CMNET\"\r"
//set ppp connnet 
#define AT_LSIPCALL_1            "AT+LSIPCALL=1\r"
//look up ppp connnect status
#define AT_LSIPCALL$            "AT+LSIPCALL?\r"
//create socket and  connect remote network address
//1: socket id 5000: local port 223.47.251.33:remote ip address 
//60000: remote port number 0:TCP protocal(1:UDP protocal)
#define AT_LSIPOPEN  "AT+LSIPOPEN=1,5000,\"223.47.251.33\",60000,0\r"

//Command to set send and receive data as HEX
#define AT_LSIPHEX1  "AT+LSIPHEX=1"
//send data to socket cache
#define AT_LSIPSEND1  "AT+LSIPSEND=1"
//send data to remote address
#define AT_LSIPPUSH1  "AT+LSIPPUSH=1\r"

//disconnect TCP/IP


#endif