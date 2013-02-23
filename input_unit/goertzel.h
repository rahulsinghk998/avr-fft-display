/*
 * goerztel.h
 * Goertzel Algorithm header file
 * targets 156, 250, 500, 1000, 2000, 4000, 8000, 16000 Hz freq
 *
 * by William Moy
 */

#ifndef GOERTZEL_H
#define GOERTZEL_H

#include "types.h"
#include <avr/pgmspace.h>

void goertzel_process_sample(BYTE);
BYTE goertzel_is_ready(void);
void goertzel_process_magnitudes(DWORD*);
void goertzel_reset(void);

#endif
