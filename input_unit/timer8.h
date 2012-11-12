#ifndef TIMER8_H
#define TIMER8_H

#include "timer_common.h"
#include "types.h"

extern volatile BYTE timer8Hits;

void startTimer8(BYTE countTo, BYTE clockSelect);
void stopTimer8(void);
BYTE isTimer8Tripped(void);
BYTE isTimer8Running(void);
void clearTimer8Tripped(void);
BYTE getTimer8Value(void);
void enableTimer8Interrupt(void);
void disableTimer8Interrupt(void);

#endif
