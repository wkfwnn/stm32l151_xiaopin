#include "stdio.h"
#include "FreeRTOS.h"
#include "string.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)

#define DBG_LOG (printf("[%u ms] %s line:%u \t",  xTaskGetTickCount(), __FILENAME__, __LINE__), printf)
#define DBG_LOG_ISR (printf("[%u ms] %s line:%u \t", xTaskGetTickCountFromISR(), __FILENAME__, __LINE__), printf)
