#ifndef TIMER16_H
#define TIMER16_H

#include "timer_common.h"
#include "types.h"

extern volatile BYTE timer16Hits;

void startTimer16(DWORD countTo, BYTE clockSelect);
void stopTimer16(void);
BYTE isTimer16Tripped(void);
BYTE isTimer16Running(void);
void clearTimer16Tripped(void);
DWORD getTimer16Value(void);
void enableTimer16Interrupt(void);
void disableTimer16Interrupt(void);

#endif
