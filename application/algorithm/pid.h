#ifndef _PID_H_
#define _PID_H_

#include "stdint.h"

struct PID_Object {
	float output;
	float error;
	float last_error;
	float measure;
	float target;
};

struct PID_Parameter {
	float sample_time;
	float kp;
	float ki;
	float kd;
	float diff_filter_param;
	float output_Min;
	float output_Max;
};


#endif //_PID_H_

