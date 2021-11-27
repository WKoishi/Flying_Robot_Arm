#ifndef _MY_TIME_H_
#define _MY_TIME_H_

#include "stdint.h"

struct Time_Object {
	volatile float Last_Time;
	volatile float Now_Time;
	volatile float Time_Delta;
	volatile uint16_t Time_Delta_INT; //µ¥Î»ms
};

void get_time_period(struct Time_Object *time_obj);


#endif // _TIME_H_

