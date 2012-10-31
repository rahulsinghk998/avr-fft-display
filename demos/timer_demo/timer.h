#ifndef TIMER_H
#define TIMER_H

#include "types.h"

extern int timerHits;

enum TIMER_CLOCK_SELECT {
	DISABLE_TIMER,
	CLOCK_SCALE_1,
	CLOCK_SCALE_8,
	CLOCK_SCALE_64,
	CLOCK_SCALE_256,
	CLOCK_SCALE_1024,
	EXT_CLOCK_FALLING,
	EXT_CLOCK_RISING
};

void start_timer(int countTo, int clockSelect);
void stop_timer(void);
BYTE get_timer_value(void);


#endif
