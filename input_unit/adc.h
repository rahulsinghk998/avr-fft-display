/*
 * adc.h
 * ADC header file, includes ADC function declarations and enum definitions
 * by William Moy
 */

#ifndef ADC_H
#define ADC_H

#include "types.h"
#include <avr/io.h>

// ADC prescaler options: select a F_cpu clock divider
#define ADC_CLK_DIV_MASK    0b00000111
#define ADC_CLK_DIV_2       0b00000001
#define ADC_CLK_DIV_4       0b00000010
#define ADC_CLK_DIV_8       0b00000011
#define ADC_CLK_DIV_16      0b00000100
#define ADC_CLK_DIV_32      0b00000101
#define ADC_CLK_DIV_64      0b00000110
#define ADC_CLK_DIV_128     0b00000111

// ADC output result alignment options
#define ADC_ALIGN_MASK      0b00010000
#define ADC_RIGHT_ALIGN     0b00000000
#define ADC_LEFT_ALIGN      0b00010000

#define ADC_CHANNEL_MASK    0b00011111
#define ADC0_SINGLE         0b00000000
#define ADC1_SINGLE         0b00000001
#define ADC2_SINGLE         0b00000010
#define ADC3_SINGLE         0b00000011
#define ADC4_SINGLE         0b00000100
#define ADC5_SINGLE         0b00000101
#define ADC6_SINGLE         0b00000110
#define ADC7_SINGLE         0b00000111

// ADC input channel select options
/*enum ADC_CHANNEL_OPTIONS {
    ADC0_ADC0_1,
    ADC0_ADC1_1,
    ADC0_ADC3_1,
    ADC1_ADC0_1,
    ADC1_ADC2_1,
    ADC1_ADC3_1,
    ADC2_ADC1_1,
    ADC2_ADC3_1,
    ADC3_ADC0_1,
    ADC3_ADC1_1,
    ADC3_ADC2_1,
    ADC3_ADC3_1,
    ADC3_ADC4_1,
    ADC3_ADC5_1,
    ADC3_ADC6_1,
    ADC3_ADC7_1,
    ADC4_ADC3_1,
    ADC4_ADC5_1,
    ADC5_ADC3_1,
    ADC5_ADC4_1,
    ADC5_ADC6_1,
    ADC6_ADC3_1,
    ADC6_ADC5_1,
    ADC6_ADC7_1,
    ADC7_ADC3_1,
    ADC7_ADC6_1,
    ADC7_ADC7_1,
    ADC0_ADC0_20,
    ADC0_ADC1_20,
    ADC0_ADC3_20,
    ADC1_ADC0_20,
    ADC1_ADC2_20,
    ADC1_ADC3_20,
    ADC2_ADC1_20,
    ADC2_ADC3_20,
    ADC3_ADC0_20,
    ADC3_ADC1_20,
    ADC3_ADC2_20,
    ADC3_ADC3_20,
    ADC3_ADC4_20,
    ADC3_ADC5_20,
    ADC3_ADC6_20,
    ADC3_ADC7_20,
    ADC4_ADC3_20,
    ADC4_ADC5_20,
    ADC5_ADC3_20,
    ADC5_ADC4_20,
    ADC5_ADC6_20,
    ADC6_ADC3_20,
    ADC6_ADC5_20,
    ADC6_ADC7_20,
    ADC7_ADC3_20,
    ADC7_ADC6_20,
    ADC7_ADC7_20
};*/

void adc_init(void);
void adc_set_prescaler(BYTE);
void adc_set_align(BYTE);
void adc_select(BYTE);
void adc_enable_int(void);
void adc_disable_int(void);
DWORD adc_get_value16(void);
BYTE adc_get_value8(void);
void adc_start(void);
void adc_clear_flag(void);
BYTE adc_is_running(void);


#endif
