#ifndef PORTMAP_ATTINY84_H
#define PORTMAP_ATTINY84_H

/* Port B */
#define XTAL1_BIT_B 0x01 // pin 2
#define XTAL2_BIT_B 0x02 // pin 3
// RESET            0x04 // pin 4
#define PB_BIT_B    0x08 // pin 5
// NC               0x10 // pin 6
#define SDA_BIT_B   0x20 // pin 7

/* Port A */
#define ADC1_BIT_A  0x01 // pin 13
#define LED1_BIT_A  0x02 // pin 12
#define ADC2_BIT_A  0x04 // pin 11
#define LED2_BIT_A  0x08 // pin 10
#define SCL_BIT_A   0x10 // pin 9
// NC               0x20 // pin 8

#endif
