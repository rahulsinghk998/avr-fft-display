#ifndef SUART_H
#define SUART_H

#include "types.h"

void suart_init(void);
void suart_xmit(BYTE);
BYTE suart_rcvr(void);

#endif	/* SUART */
