#ifndef TIMER16_H
#define TIMER16_H

#include "timer_common.h"
#include "types.h"

void timer16_init(DWORD countTo, BYTE clockSelect);
void timer16_start(void);
void timer16_stop(void);
BYTE timer16_is_tripped(void);
BYTE timer16_is_running(void);
void timer16_clear_flag(void);
DWORD timer16_get_count(void);
void timer16_enable_int(void);
void timer16_disable_int(void);

#endif
