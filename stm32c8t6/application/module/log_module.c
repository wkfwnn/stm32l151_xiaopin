#include "stdio.h"


#define DBG_LOG (printf("%s:%u [%ld]\t", __FILE__, __LINE__, xTaskGetTickCount()), printf)