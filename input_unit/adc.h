/*
 * adc.h
 * ADC header file, includes ADC function declarations and enum definitions
 * by William Moy
 */

#ifndef ADC_H
#define ADC_H

#include "types.h"

// ADC prescaler options: select a F_cpu clock divider
enum ADC_PRESCALER_OPTIONS {
    ADC_CLK_DIV_2,
    ADC_CLK_DIV_4,
    ADC_CLK_DIV_8,
    ADC_CLK_DIV_16,
    ADC_CLK_DIV_32,
    ADC_CLK_DIV_64,
    ADC_CLK_DIV_128
};

// ADC output result alignment options
enum ADC_ALIGN_OPTIONS {
    ADC_LEFT_ALIGN,
    ADC_RIGHT_ALIGN
};

// ADC input channel select options
enum ADC_CHANNEL_OPTIONS {
    ADC0,
    ADC1,
    ADC2,
    ADC3,
    ADC4,
    ADC5,
    ADC6,
    ADC7,
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
};

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
