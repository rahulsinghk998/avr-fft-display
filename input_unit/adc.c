/*
 *
 *
 *
 */

#include <avr/io.h>
#include "adc.h"
#include "common.h"

void adc_init(void) {
    bit_set(ADCSRA, ADEN);
}

void adc_set_prescaler(BYTE prescaler) {
    ADCSRA &= 0xF8; // Zero out the biis 0-2
    switch (prescaler) {
        case ADC_CLK_DIV_2:
            ADCSRA |= 1;
            break;
        case ADC_CLK_DIV_4:
            ADCSRA |= 2;
            break;
        case ADC_CLK_DIV_8:
            ADCSRA |= 3;
            break;
        case ADC_CLK_DIV_16:
            ADCSRA |= 4;
            break;
        case ADC_CLK_DIV_32:
            ADCSRA |= 5;
            break;
        case ADC_CLK_DIV_64:
            ADCSRA |= 6;
            break;
        case ADC_CLK_DIV_128:
            ADCSRA |= 7;
            break;
    }
}

void adc_set_align(BYTE alignOption) {
    switch (alignOption) {
        case (ADC_LEFT_ALIGN):
            bit_set(ADCSRB, ADLAR);
            break;
        case (ADC_RIGHT_ALIGN):
            bit_clear(ADCSRB, ADLAR);
            break;
        default:
            bit_clear(ADCSRB, ADLAR);
            break;
    }
}

void adc_select(BYTE inputOption) {
    ADMUX &= 0xC0; // Zero out the bits 0-5
    switch (inputOption) {
        case ADC0_ADC3_1:
            ADMUX |= 0x0A;
            break;
        case ADC1_ADC3_1:
            ADMUX |= 0x0E;
            break;
        case ADC0_ADC3_20:
            ADMUX |= 0x0B;
            break;
        case ADC1_ADC3_20:
            ADMUX |= 0x0F;
            break;
        default:
            ADMUX |= 0x20; // compare to GND
            break;
    }
}

void adc_enable_int(void) {
    bit_set(ADCSRA, ADIE);
}

void adc_disable_int(void) {
    bit_clear(ADCSRA, ADIE);
}

DWORD adc_get_value16(void) {
    if (bit_is_clear(ADCSRB, ADLAR))
        return (DWORD)ADCL & ((DWORD)ADCH << 8);
    else
        return ((DWORD)ADCH << 2) & ((DWORD)ADCL >> 6);
}

BYTE adc_get_value8(void) {
    if (bit_is_set(ADCSRB, ADLAR))
        return ADCH;
    else
        return (ADCL >> 2) & (ADCH << 6);
}

void adc_start(void) {
    bit_set(ADCSRA, ADSC);
}

void adc_clear_flag(void) {
    bit_set(ADCSRA, ADIF);
}

BYTE adc_is_running(void) {
    return bit_is_set(ADCSRA, ADSC);
}
