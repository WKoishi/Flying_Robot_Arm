#ifndef _PID_H_
#define _PID_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif //_PID_H_

