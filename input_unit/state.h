/*
 * state.h
 * State variables for avr-fft-display, currently underutilized.  Only holds
 * which channel should take input.  May be used for expansion.
 * by William Moy
 */

#ifndef STATE_H
#define STATE_H

#include "types.h"

extern volatile BYTE state;

enum STATE_BITS {
    PB_STATE = 1
    // other state variables go here
};

#endif
