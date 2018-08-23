#ifndef _A9500_MODULE_AT_COMMOND_H_
#define _A9500_MODULE_AT_COMMOND_H_

/****************************************************************************
			A9500    AT COMMOND instruction
	IOT module we use A9500,module instructtion set we support GSM Rec.07.07、07.05、
 3GPP TS 27.005、27.007 and ITU-T Rec. V25ter and instructions set of LongSung company
 send: AT<x><CR>
 receive:AT<x><CR><CF>(except for ATV0(return <CR>) and ATQ1(no return))
 note:
      @1 AT instruction syntax error, return ERROR string
      @2 AT command syntax is correct, wrong parameters,return +CME ERROR：<err>
         or +CMS ERROR:<err>
      @3 AT instructions are executed correctly,return OK
      @4 When receiving a short message, there will be a certain format string sent to the terminal
      @5 When returning an error message, different return results can be set with AT+CMEE=<...>			
note:
$:mean use ?

************************************************************************/

/*ABBREVIATION

AMR Adaptive Multi-rate 自适应多速率
BER Bit Error Rate 误码率
BTS Base Transceiver Station 基站收发信台
PCI Peripheral Component Interconnect 外设部件互连
CS Circuit Switched (CS) domain 电路域
CSD Circuit Switched Data 电路交换数据
DCE Data communication equipment 数据电路终端设备
DTE Data terminal equipment 数据终端设备
DTR Data Terminal Ready 数据终端就绪
EDGE Enhanced Data rates for GSM Evolution 增强型 GPRS
eDRX Extended Discontinuous Reception 扩展非连续接收
EFR Enhanced Full Rate 增强型全速率
EGSM Enhanced GSM 增强型 GSM
EMC Electromagnetic Compatibility 电磁兼容性
ESD Electrostatic Discharge 静电释放
FR Frame Relay 帧中继
GMSK Gaussian Minimum Shift Keying 高斯最小移频键控
GPIO General Purpose Input Output 通用输入/输出
GPRS General Packet Radio Service 通用分组无线系统
GSM Global Standard for Mobile Communications 全球标准移动通信系统
HR Half Rate 半速
HSDPA High Speed Downlink Packet Access 高速下行分组接入
HSUPA High Speed Uplink Packet Access 高速上行分组接入
HSPA HSPA High-Speed Packet Access 高速分组接入
IEC International Electrote chnical Commission 国际电工技术委员会
IMEI International Mobile Equipment Identity 国际移动设备标识
I/O Input/Output 输入/输出
ISO International Standards Organization 国际标准化组织
ITU International Telecommunications Union 国际电信联盟
bps bits per second 比特每秒
LED Light Emitting Diode 发光二极管
LTE Long Term Evolution 长期演进技术
M2M Machine to machine 机器到机器
MO Mobile Originated 移动台发起的
MT Mobile Terminated 移动台终止的
NB-IoT Narrow Band Internet of Things 窄带物联网
NTC Negative Temperature Coefficient 负温度系数
PC Personal Computer 个人计算机

PCB Printed Circuit Board 印制电路板
PCS Personal Cellular System 个人蜂窝系统
PCI Peripheral Component Interconnect 外设部件互连
PCM Pulse Code Modulation 脉冲编码调制
PCS Personal Communication System GSM1900
PDU Packet Data Unit 分组数据单元
PPP Point-to-point protocol 点到点协议
PS Packet Switched 分组交换
PSM Power Saving Mode PSM 省电模式
PTW Paging Time Window 寻呼时间窗
QPSK Quadrate Phase Shift Keying 正交相位移频键控
SIM Subscriber Identity Module 用户识别模块
TE Terminal Equipment 终端设备（同 DTE）
TA Terminal Adaptor 终端适配器（同 DCE）
TCP/IP Transmission Control Protocol/ Internet Protocol 传输控制协议/互联网协议
UART Universal asynchronous receiver-transmitter 通用异步收/发器(机)
USIM Universal Subscriber Identity Module 通用用户识别模块
UMTS Universal Mobile Telecommunications System 通用移动通信系统
USB Universal Serial Bus 通用串行总线
WCDMA Wideband Code Division Multiple Access 宽带码分多址*/

/**********************************************************************
					Basic AT commond 
**********************************************************************/

//The command is used to check whether the DCE module can work properly
#define AT         "AT\r"
//mean A/,Repeat previous instruction line
#define A_          "A/\r"
// AT&F Restore all TA parameters to factory configuration
#define AT_F        "AT&F\r"
//Save user configuration parameters(AT&W)
#define AT_W        "AT&W\r"
//Instruction echo mode
#define ATE         "ATE\r"
//TA's Manufacturer Information
#define ATI         "ATI\r"
//Set whether TA sends result code to TE
#define ENATQ        "ATQ1\r"
#define DISATQ       "ATQ1\r"
//format of TA's return 
#define TEXTATV     "ATV1\r"     
#define NUMATV      "ATV0\r"
//You can set whether the TA sends a specific result code to the TE(para can be 0-4)
/*
[0] 进入在线数据状态时即给定连接结果码。拨号音和遇忙检测均禁用。
1 进入在线数据状态时即给定连接<text>结果码。拨号音和遇忙检测均禁用。
2 进入在线数据状态时即给定连接<text>结果码。遇忙检测禁用，可启用拨号音。
3 进入在线数据状态时即给定连接<text>结果码。拨号音检测禁用，但可启用遇忙检测。
4 进入在线数据状态时即给定连接<text>结果码。拨号音和遇忙检测均可启用。
*/
#define ATX0         "ATX0\r"
#define ATX1         "ATX1\r"
#define ATX2         "ATX2\r"
#define ATX3         "ATX3\r"
//Restore configuration saved by AT&W
#define ATZ          "ATZ\r"
//Set the number of rings before auto answer(default is 0)
#define ATS0         "ATS0=0\r"
//Set AT command line terminator(default is <CR>,asiic is 13)
#define ATS3         "ATS3=13\r"
//Set the character to get result code and info text(default is <LF> asiic is 10)
#define ATS4         "ATS4=10\r"
//Set character to delete characters in command line(default remove BackSpace character,asicc is 8)
#define ATS5         "ATS5=8\r"
//Delay control before blind dialing(default is 2 seconds)
#define ATS6         "ATS6=2\r"
//Wait for the call to complete(default is 0 seconds)
#define ATS7         "ATS7=0\r"
//Pause dialing(default is 2 seconds)
#define ATS8         "ATS8=2\r"
//Hang-up delay(default is 14 *100ms(unit is 100 ms))
#define ATS10        "ATS10=14\r"
//Query the manufacturer's name
#define AT_CGMI      "AT+CGMI\r"
#define AT_GMI       "AT+GMI\r"
//Inquiry Module Model(AT+CGMM/GMM)
#define AT_CGMM      "AT+CGMM\r"
#define AT_GMM       "AT+GMM\r"
//Query module version information(AT+CGMR/GMR)
#define AT_CGMR       "AT+CGMR\r"
#define AT_GMR        "AT+GMR\r"
//Version number query
#define AT_LCTSW      "AT+LCTSW\r"
//Version compile time query
#define AT_BDTIME     "AT+BDTIME\r"
//Query product IMEI number
#define AT_CGSN       "AT+CGSN\r"
#define AT_GSN        "AT+GSN\r"
//Query International Mobile Station Equipment Identity IMSI Numbers
//DCE读取移动设备上的USIM/SIM内附的IMSI号,error:USIM/SIM卡不在位，未初始化完毕或者SIM被锁，需要输入PIN码或 PUK码解锁
#define AT_CIMI      "AT+CIMI\r"
//Query IC card identification code instructions
#define AT_ICCID     "AT+ICCID\r"
//return User number
#define AT_CNUM      "AT+CNUM\r"
//
#define AT_CRSM      "AT+CRSM\r"




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
//look up active socket connect
#define AT_LSIPCLOSE$ "AT+LSIPCLOSE?\r"
//close active socket 1
#define AT_LSIPCLOSE1 "AT+LSIPCLOSE=1\r"
//close ppp connect 
#define AT_LSIPCALL0  "AT+LSIPCALL=0\r"




//GPS  function
//set gps mode
#define AT_GPSMODE1 "AT+GPSMODE=1\r"
//set gps para
#define AT_GPSCONFIG_PARA "AT+GPSCONFIG=20,180,9999999,1,2,1\r"
//set gps start
#define AT_GPSSTART "AT+ GPSSTART\r"
//end gps
#define AT_GPSEND   "AT+GPSEND"




//lower psm function







//eDRX function





//COAP function

#endif