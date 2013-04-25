/*
 * timer8.h
 * 8-bit timer header file
 * by William Moy
 */

#ifndef TIMER8_H
#define TIMER8_H

#include "timer_common.h"
#include "types.h"

void timer8_init(BYTE countTo, BYTE clockSelect);
void timer8_start(void);
void timer8_stop(void);
BYTE timer8_is_tripped(void);
BYTE timer8_is_running(void);
void timer8_clear_flag(void);
BYTE timer8_get_count(void);
void timer8_enable_int(void);
void timer8_disable_int(void);

#endif
