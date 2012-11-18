/*
 *
 *
 *
 */

#include <avr/io.h>
#include "common.h"
#include "twi_attiny84.h"


void twi_init_master(void) {
    // Set pins as outputs
    bit_set(DDRA, SCL_BIT);
    bit_set(DDRA, SDA_BIT);

    // Set pins as source
    bit_set(PORTA, SCL_BIT);
    bit_set(PORTA, SDA_BIT);
}

void twi_master_send(BYTE* msg, BYTE msgLen) {

}
