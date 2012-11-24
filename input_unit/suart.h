/*
 * suart.h
 * Software UART header file, interface for elm-chan's SUART asm code.
 */

#ifndef SUART_H
#define SUART_H

#include "types.h"

void suart_init(void);
void suart_xmit(BYTE);
BYTE suart_rcvr(void);

#endif	/* SUART */
