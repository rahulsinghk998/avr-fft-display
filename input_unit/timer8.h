#ifndef TIMER8_H
#define TIMER8_H

#include "types.h"

extern volatile BYTE timerHits;

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

void startTimer(BYTE countTo, BYTE clockSelect);
void stopTimer(void);
BYTE isTimerTripped(void);
BYTE isTimerRunning(void);
void clearTimerTripped(void);
BYTE getTimerValue(void);
void enableTimerInterrupt(void);
void disableTimerInterrupt(void);

#endif
