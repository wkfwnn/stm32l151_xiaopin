#ifndef __SPEAK_MODULE_H_
#define __SPEAK_MODULE_H_

#include "cmsis_os.h"


typedef enum {
	SPEAK_STATUS_NONE,
		
	SPEAK_STATUS_ALARM,
	SPEAK_STATUS_MUTE,
	
	SPEAK_STATUS_END
	
}speak_status_t;



void set_speak_out_put(uint8_t status);


#endif

