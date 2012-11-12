#ifndef PORTMAP_ATTINY84_H
#define PORTMAP_ATTINY84_H

/* Port B */
#define XTAL1_BIT     0 // 0x01 // pin 2
#define XTAL2_BIT     1 // 0x02 // pin 3
// RESET              3 // 0x08 // pin 4
#define PB_BIT        2 // 0x04 // pin 5

/* Port A */
#define ADC1_BIT      0 // 0x01 // pin 13
#define LED1_BIT      1 // 0x02 // pin 12
#define ADC2_BIT      2 // 0x04 // pin 11
#define LED2_BIT      3 // 0x08 // pin 10
#define SCL_BIT       4 // 0x10 // pin 9
// NC                 5 // 0x20 // pin 8
#define SDA_BIT       6 // 0x40 // pin 7
// NC                 7 // 0x80 // pin 6

#endif
