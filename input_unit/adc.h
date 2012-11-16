/*
 *
 *
 *
 */

#ifndef ADC_H
#define ADC_H

void adc_init(void);
void adc_select(BYTE);
void adc_enable_int(void);
void adc_disable_int(void);
DWORD adc_get_value16(void);
BYTE adc_get_value8(void);

#endif
